#ifdef ANDROID
//
//  AEXMLHttpRequestBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/1.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEManager.hpp"
#include "AEXMLHttpRequest.hpp"
#include "AEXMLHttpRequestBean.hpp"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_app_engine_AEXMLHttpRequestImpl_onFinish(JNIEnv* jni, jobject jthis, jlong thiz, jstring mime_, jstring type_, jbyteArray data_) {
	const auto bean = reinterpret_cast<AEXMLHttpRequestBean*>(thiz);
	const char* mime = jni->GetStringUTFChars(mime_, NULL);
	const char* type = jni->GetStringUTFChars(type_, NULL);

	jsize  size = jni->GetArrayLength(data_);
	jbyte* data = jni->GetByteArrayElements(data_, 0);
	bean->onFinish(size, data, mime, type);

	jni->ReleaseStringUTFChars(mime_, mime);
	jni->ReleaseStringUTFChars(type_, type);
	jni->ReleaseByteArrayElements(data_, data, 0);
}

#ifdef __cplusplus
}
#endif

AEXMLHttpRequestBean::~AEXMLHttpRequestBean() {
	abort();
	AEJNI::jni()->DeleteGlobalRef(impl);
}
AEXMLHttpRequestBean::AEXMLHttpRequestBean(AEXMLHttpRequest* request) {
	this->impl = nullptr;
	this->data = nullptr;
	this->manager = nullptr;
	this->request = request;
	reset(true, nullptr, nullptr);
}
void AEXMLHttpRequestBean::init(AEManager* mgr) {
	this->manager = mgr;
	static const char name[] = "app/engine/AEXMLHttpRequestImpl";
	static const char sign[] = "(JLapp/engine/AEView;)V";
	AEJNI::jnew(name, sign, [this](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(this->manager);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}
void AEXMLHttpRequestBean::open(const char* method_, const char* url_, const bool async, const char* username_, const char* password_) {
	reset(async, url_, method_);
	static const char name[] = "open";
	static const char sign[] = "(Ljava/lang/String;Ljava/lang/String;ZLjava/lang/String;Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [=](JNIEnv* jni, jobject thiz, jmethodID func) {
		jstring method   = jni->NewStringUTF(method_);
		jstring url      = jni->NewStringUTF(url_);
		jstring username = jni->NewStringUTF(username_);
		jstring password = jni->NewStringUTF(password_);

		jni->CallVoidMethod(thiz, func, method, url, async, username, password);

		jni->DeleteLocalRef(method);
		jni->DeleteLocalRef(url);
		jni->DeleteLocalRef(username);
		jni->DeleteLocalRef(password);
	});
}
void AEXMLHttpRequestBean::send() {
	static const char name[] = "send";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject view, jmethodID method) {
		jni->CallVoidMethod(view, method);
	});
}
void AEXMLHttpRequestBean::abort() {
	static const char name[] = "abort";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEXMLHttpRequestBean::reset(bool async, const char* url, const char* method) {
	if (this->data) {
		free(this->data);
	}
	this->data			= nullptr;
	this->size			= 0;
	this->status		= 0;
	this->timeout		= 3000;
	this->readyState	= 0;
	this->method		= method ? : "GET";
	this->mimeType		= "";
	this->statusText	= "";
	this->responseURL	= url ? : "";
	this->responseType	= "";
	this->reqHeaders.clear();
	this->retHeaders.clear();

	static const char name[] = "reset";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject target, jmethodID method) {
		jni->CallVoidMethod(target, method);
	});
}
void AEXMLHttpRequestBean::overrideMimeType(const char* mimetype_) {
	mimeType = mimetype_;
	static const char name[] = "overrideMimeType";
	static const char sign[] = "(Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [mimetype_](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring mimetype = jni->NewStringUTF(mimetype_);
		jni->CallVoidMethod(obj, method, mimetype);
		jni->DeleteLocalRef(mimetype);
	});
}
void AEXMLHttpRequestBean::setRequestHeader(const char* key_, const char* val_) {
	reqHeaders[key_] = val_;
	static const char name[] = "setRequestHeader";
	static const char sign[] = "(Ljava/lang/String;Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [key_, val_](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring key = jni->NewStringUTF(key_);
		jstring val = jni->NewStringUTF(val_);
		jni->CallVoidMethod(obj, method, key, val);
		jni->DeleteLocalRef(key);
		jni->DeleteLocalRef(val);
	});
}
void AEXMLHttpRequestBean::onFinish(int size, const void* data, const char* mime, const char* type) {
	this->size = size;
	this->data = malloc(size);
    this->status       = 200;
	this->mimeType     = mime;
	this->responseType = type;
	memcpy(this->data, data, size);
	this->request->onFinish();
}

std::string AEXMLHttpRequestBean::getResponseHeader(const char* key) {
	auto it = retHeaders.find(key);
	if (it == retHeaders.end()) {
		return "";
	}
	else {
		return it->second;
	}
}
std::string AEXMLHttpRequestBean::getAllResponseHeaders() {
	return "";
}

uint AEXMLHttpRequestBean::getStatus() {
	return status;
}
uint AEXMLHttpRequestBean::getTimeout() {
	return timeout;
}
uint AEXMLHttpRequestBean::getReadyState() {
	return readyState;
}
std::string AEXMLHttpRequestBean::getMimeType() {
	return mimeType;
}
std::string AEXMLHttpRequestBean::getStatusText() {
	return statusText;
}
std::string AEXMLHttpRequestBean::getResponseURL() {
	return responseURL;
}
std::string AEXMLHttpRequestBean::getResponseType() {
	return responseType;
}
void AEXMLHttpRequestBean::setResponseType(const char* type_) {
	responseType = type_;
	static const char name[] = "overrideResponseType";
	static const char sign[] = "(Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [type_](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring type = jni->NewStringUTF(type_);
		jni->CallVoidMethod(obj, method, type);
		jni->DeleteLocalRef(type);
	});
}

#endif
