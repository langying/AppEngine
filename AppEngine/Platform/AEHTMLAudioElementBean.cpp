#ifdef ANDROID
//
//  AEHTMLAudioElementBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "AEJNI.hpp"
#include "AEHTMLAudioElement.hpp"
#include "AEHTMLAudioElementBean.hpp"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_app_engine_AEHTMLAudioElementImpl_onFinish(JNIEnv* jni, jobject jplayer, jlong thiz, jint width, jint height, jint duration) {
	AEHTMLAudioElementBean* bean = reinterpret_cast<AEHTMLAudioElementBean*>(thiz);
	bean->audio->onFinish(width, height, duration);
}

#ifdef __cplusplus
}
#endif

AEHTMLAudioElementBean::~AEHTMLAudioElementBean() {
	AEJNI::jni()->DeleteGlobalRef(impl);
}
AEHTMLAudioElementBean::AEHTMLAudioElementBean(AEHTMLAudioElement* audio) {
    this->mgr   = nullptr;
	this->impl  = nullptr;
	this->audio = audio;
}

void AEHTMLAudioElementBean::init(AEManager* mgr) {
    this->mgr = mgr;
	static const char name[] = "app/engine/AEHTMLAudioElementImpl";
	static const char sign[] = "(JLapp/engine/AEView;)V";
	AEJNI::jnew(name, sign, [this](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(this->mgr);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}

void AEHTMLAudioElementBean::load() {
	static const char name[] = "load";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLAudioElementBean::play() {
	static const char name[] = "play";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLAudioElementBean::stop() {
	static const char name[] = "stop";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLAudioElementBean::release() {
	static const char name[] = "release";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}

std::string AEHTMLAudioElementBean::getSrc() {
	static const char name[] = "getSrc";
	static const char sign[] = "()Ljava/lang/String;";
	std::string val;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring src_ = (jstring)jni->CallObjectMethod(obj, method);
		const char* src = jni->GetStringUTFChars(src_, nullptr);
		val = src;
		jni->ReleaseStringUTFChars(src_, src);
	});
	return val;
}
void AEHTMLAudioElementBean::setSrc(const char* val) {
	static const char name[] = "setSrc";
	static const char sign[] = "(Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring val_ = jni->NewStringUTF(val);
		jni->CallVoidMethod(obj, method, val_);
		jni->DeleteLocalRef(val_);
	});
}
bool AEHTMLAudioElementBean::getLoop() {
	static const char name[] = "getLoop";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setLoop(bool val) {
	static const char name[] = "setLoop";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
bool AEHTMLAudioElementBean::getMuted() {
	static const char name[] = "getMuted";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setMuted(bool val) {
	static const char name[] = "setMuted";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
bool AEHTMLAudioElementBean::getPaused() {
	static const char name[] = "getPaused";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setPaused(bool val) {
	static const char name[] = "setPaused";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
bool AEHTMLAudioElementBean::getAutoplay() {
	static const char name[] = "getAutoplay";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setAutoplay(bool val) {
	static const char name[] = "setAutoplay";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
uint AEHTMLAudioElementBean::getCurrentTime() {
	static const char name[] = "getCurrentTime";
	static const char sign[] = "()I";
	uint val = 0;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallIntMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setCurrentTime(uint val) {
	static const char name[] = "setCurrentTime";
	static const char sign[] = "(I)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
float AEHTMLAudioElementBean::getVolume() {
	static const char name[] = "getVolume";
	static const char sign[] = "()F";
	float val = 1.0;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallFloatMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setVolume(float val) {
	static const char name[] = "setVolume";
	static const char sign[] = "(F)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
float AEHTMLAudioElementBean::getPlaybackRate() {
	static const char name[] = "getPlaybackRate";
	static const char sign[] = "()F";
	float val = 1.0;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallFloatMethod(obj, method);
	});
	return val;
}
void AEHTMLAudioElementBean::setPlaybackRate(float val) {
	static const char name[] = "setPlaybackRate";
	static const char sign[] = "(F)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}

#endif /* ANDROID */
