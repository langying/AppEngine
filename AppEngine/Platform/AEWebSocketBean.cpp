#ifdef ANDROID
//
//  AEWebSocketBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEWebSocket.hpp"
#include "AEWebSocketBean.hpp"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_app_engine_AEWebSocketImpl_onopen(JNIEnv* jni, jobject obj, jlong thiz, jint status, jstring message_) {
	const char* message = jni->GetStringUTFChars(message_, NULL);
	reinterpret_cast<AEWebSocketBean*>(thiz)->onopen(status, message);
	jni->ReleaseStringUTFChars(message_, message);
}
JNIEXPORT void JNICALL
Java_app_engine_AEWebSocketImpl_onclose(JNIEnv* jni, jobject obj, jlong thiz, jint code, jstring reason_, jboolean remote) {
	const char* reason = jni->GetStringUTFChars(reason_, NULL);
	reinterpret_cast<AEWebSocketBean*>(thiz)->onclose(code, reason, remote);
	jni->ReleaseStringUTFChars(reason_, reason);
}
JNIEXPORT void JNICALL
Java_app_engine_AEWebSocketImpl_onerror(JNIEnv* jni, jobject obj, jlong thiz, jstring message_) {
	const char* message = jni->GetStringUTFChars(message_, NULL);
	reinterpret_cast<AEWebSocketBean*>(thiz)->onerror(message);
	jni->ReleaseStringUTFChars(message_, message);
}
JNIEXPORT void JNICALL
Java_app_engine_AEWebSocketImpl_onmessage(JNIEnv* jni, jobject obj, jlong thiz, jstring message_) {
	const char* message = jni->GetStringUTFChars(message_, NULL);
	reinterpret_cast<AEWebSocketBean*>(thiz)->onmessage(message);
	jni->ReleaseStringUTFChars(message_, message);
}

#ifdef __cplusplus
}
#endif

AEWebSocketBean::~AEWebSocketBean() {
    close(0, "destructor");
    AEJNI::jni()->DeleteGlobalRef(impl);
}
AEWebSocketBean::AEWebSocketBean(AEWebSocket* socket) {
	this->impl = nullptr;
	this->manager = nullptr;
	this->webSocket = socket;
}

void AEWebSocketBean::init(AEManager* mgr, const char* url_, const char* ptl_) {
	this->manager = mgr;
	static const char name[] = "app/engine/AEWebSocketImpl";
	static const char sign[] = "(JLapp/engine/AEView;Ljava/lang/String;Ljava/lang/String;)V";
	AEJNI::jnew(name, sign, [this, url_, ptl_](JNIEnv* jni, jclass clazz, jmethodID method) {
		jstring url  = jni->NewStringUTF(url_);
		jstring ptl  = jni->NewStringUTF(ptl_);
		jobject view = AEJNI::viewWithMgr(this->manager);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view, url, ptl);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
		jni->DeleteLocalRef(url);
		jni->DeleteLocalRef(ptl);
	});
}

void AEWebSocketBean::send(const char* text_) {
	static const char name[] = "send";
	static const char sign[] = "(Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [text_](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring text = jni->NewStringUTF(text_);
		jni->CallVoidMethod(obj, method, text);
		jni->DeleteLocalRef(text);
	});
}
void AEWebSocketBean::send(const void* data_, int size) {
	static const char name[] = "send";
		static const char sign[] = "([B)V";
		AEJNI::call(impl, name, sign, [data_, size](JNIEnv* jni, jobject obj, jmethodID method) {
			jbyteArray data = jni->NewByteArray(size);
			jni->SetByteArrayRegion(data, 0, size, static_cast<const jbyte*>(data_));
			jni->CallVoidMethod(obj, method, data);
			jni->DeleteLocalRef(data);
		});
}
void AEWebSocketBean::close(uint code, const char* reason_) {
	static const char name[] = "close";
	static const char sign[] = "(ILjava/lang/String;)V";
	AEJNI::call(impl, name, sign, [code, reason_](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring reason = jni->NewStringUTF(reason_);
		jni->CallVoidMethod(obj, method, code, reason);
		jni->DeleteLocalRef(reason);
	});
}

void AEWebSocketBean::onopen(int status, const char* message) {
	webSocket->onopen(message, status);
}
void AEWebSocketBean::onclose(int code, const char* reason, bool remote) {
	webSocket->onclose(reason, code, remote);
}
void AEWebSocketBean::onerror(const char* message) {
	webSocket->onerror(message);
}
void AEWebSocketBean::onmessage(const char* message) {
	webSocket->onmessage(message);
}
#endif
