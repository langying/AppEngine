#ifdef ANDROID
//
//  AESoundBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AESound.hpp"
#include "AEManager.hpp"
#include "AESoundBean.hpp"

AESoundBean::~AESoundBean() {
	release();
	AEJNI::jni()->DeleteGlobalRef(bean);
}
AESoundBean::AESoundBean(AESound* sud) {
	muted	= false;
	volume	= 1;
	bean	= nullptr;
	sound	= sud;
	manager	= nullptr;
}
void AESoundBean::init(AEManager* mgr) {
	manager = mgr;
	static const char name[] = "app/engine/AESoundBean";
	static const char sign[] = "(JLapp/engine/AEView;)V";
	AEJNI::jnew(name, sign, [this](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(this->manager);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view);
		this->bean = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}
void AESoundBean::release() {
	static const char name[] = "release";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::onBackground() {
	static const char name[] = "onBackground";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::onForeground() {
	static const char name[] = "onForeground";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::vibrate() {
	static const char name[] = "vibrate";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::stopALL() {
	static const char name[] = "stopALL";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::pauseALL() {
	static const char name[] = "pauseALL";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::resumeALL() {
	static const char name[] = "resumeALL";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::setMuted(bool muted) {
	this->muted = muted;
	static const char func[] = "setMuted";
	static const char sign[] = "(Z)V";
	AEJNI::call(bean, func, sign, [muted](JNIEnv* jni, jobject canvas, jmethodID method) {
		jni->CallVoidMethod(canvas, method, muted);
	});
}
void AESoundBean::setVolume(bool volume) {
	this->volume = volume;
	static const char func[] = "setVolume";
	static const char sign[] = "(F)V";
	AEJNI::call(bean, func, sign, [volume](JNIEnv* jni, jobject canvas, jmethodID method) {
		jni->CallVoidMethod(canvas, method, volume);
	});
}
uint AESoundBean::play(const char* url_) {
	GLuint sound = 0;
	static const char func[] = "play";
	static const char sign[] = "(Ljava/lang/String;)V";
	AEJNI::call(bean, func, sign, [url_, &sound](JNIEnv* jni, jobject canvas, jmethodID method) {
		jstring url = jni->NewStringUTF(url_);
		sound = jni->CallIntMethod(canvas, method, url);
		jni->DeleteLocalRef(url);
	});
	return sound;
}
void AESoundBean::stop(const char* url) {
	static const char name[] = "stop";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::pause(const char* url) {
	static const char name[] = "pause";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::resume(const char* url) {
	static const char name[] = "resume";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESoundBean::unload(const char* url) {
	static const char name[] = "unload";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
uint AESoundBean::preload(const char* url) {
	uint val = 0;
	static const char name[] = "preload";
	static const char sign[] = "()V";
	AEJNI::call(bean, name, sign, [&val](JNIEnv* jni, jobject obj, jmethodID method) {
		val = jni->CallIntMethod(obj, method);
	});
	return val;
}
#endif
