#ifdef ANDROID
//
//  AEHTMLVideoElementBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "AEJNI.hpp"
#include "AEHTMLVideoElement.hpp"
#include "AEHTMLVideoElementBean.hpp"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_app_engine_AEHTMLVideoElementImpl_onFinish(JNIEnv* jni, jobject jplayer, jlong thiz, jint width, jint height, jint duration) {
	AEHTMLVideoElementBean* bean = reinterpret_cast<AEHTMLVideoElementBean*>(thiz);
	bean->video->onFinish(width, height, duration);
}

#ifdef __cplusplus
}
#endif

AEHTMLVideoElementBean::~AEHTMLVideoElementBean() {
	AEJNI::jni()->DeleteGlobalRef(impl);
}
AEHTMLVideoElementBean::AEHTMLVideoElementBean(AEHTMLVideoElement* video) {
    this->mgr   = nullptr;
	this->impl  = nullptr;
	this->video = video;
}

void AEHTMLVideoElementBean::init(AEManager* mgr) {
    this->mgr = mgr;
	static const char name[] = "app/engine/AEHTMLVideoElementImpl";
	static const char sign[] = "(JLapp/engine/AEView;)V";
	AEJNI::jnew(name, sign, [this](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(this->mgr);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}

void AEHTMLVideoElementBean::load() {
	static const char name[] = "load";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLVideoElementBean::play() {
	static const char name[] = "play";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLVideoElementBean::stop() {
	static const char name[] = "stop";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLVideoElementBean::release() {
	static const char name[] = "release";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AEHTMLVideoElementBean::texImage2D(GLuint texture, GLenum target, GLint level, GLint infmt, GLenum format, GLenum type) {
	static const char name[] = "texImage2D";
	static const char sign[] = "(IIIIII)V";
	AEJNI::call(impl, name, sign, [=](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, texture, target, level, infmt, format, type);
	});
}
void AEHTMLVideoElementBean::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type) {
	static const char name[] = "texSubImage2D";
	static const char sign[] = "(IIIIII)V";
	AEJNI::call(impl, name, sign, [=](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, target, level, xoffset, yoffset, format, type);
	});
}

std::string AEHTMLVideoElementBean::getSrc() {
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
void AEHTMLVideoElementBean::setSrc(const char* val) {
	static const char name[] = "setSrc";
	static const char sign[] = "(Ljava/lang/String;)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring val_ = jni->NewStringUTF(val);
		jni->CallVoidMethod(obj, method, val_);
		jni->DeleteLocalRef(val_);
	});
}
bool AEHTMLVideoElementBean::getLoop() {
	static const char name[] = "getLoop";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setLoop(bool val) {
	static const char name[] = "setLoop";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
bool AEHTMLVideoElementBean::getMuted() {
	static const char name[] = "getMuted";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setMuted(bool val) {
	static const char name[] = "setMuted";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
bool AEHTMLVideoElementBean::getPaused() {
	static const char name[] = "getPaused";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setPaused(bool val) {
	static const char name[] = "setPaused";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
bool AEHTMLVideoElementBean::getAutoplay() {
	static const char name[] = "getAutoplay";
	static const char sign[] = "()Z";
	bool val = false;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallBooleanMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setAutoplay(bool val) {
	static const char name[] = "setAutoplay";
	static const char sign[] = "(Z)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
uint AEHTMLVideoElementBean::getCurrentTime() {
	static const char name[] = "getCurrentTime";
	static const char sign[] = "()I";
	uint val = 0;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallIntMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setCurrentTime(uint val) {
	static const char name[] = "setCurrentTime";
	static const char sign[] = "(I)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
float AEHTMLVideoElementBean::getVolume() {
	static const char name[] = "getVolume";
	static const char sign[] = "()F";
	float val = 1.0;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallFloatMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setVolume(float val) {
	static const char name[] = "setVolume";
	static const char sign[] = "(F)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}
float AEHTMLVideoElementBean::getPlaybackRate() {
	static const char name[] = "getPlaybackRate";
	static const char sign[] = "()F";
	float val = 1.0;
	AEJNI::call(impl, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallFloatMethod(obj, method);
	});
	return val;
}
void AEHTMLVideoElementBean::setPlaybackRate(float val) {
	static const char name[] = "setPlaybackRate";
	static const char sign[] = "(F)V";
	AEJNI::call(impl, name, sign, [val](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method, val);
	});
}

#endif
