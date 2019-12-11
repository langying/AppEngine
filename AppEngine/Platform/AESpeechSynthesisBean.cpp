#ifdef ANDROID
//
//  AESpeechSynthesisBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/11.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEManager.hpp"
#include "AESpeechSynthesis.hpp"
#include "AESpeechSynthesisBean.hpp"

AESpeechSynthesisBean::~AESpeechSynthesisBean() {
	release();
	AEJNI::jni()->DeleteGlobalRef(impl);
}
AESpeechSynthesisBean::AESpeechSynthesisBean(AESpeechSynthesis* that) {
	this->impl = nullptr;
	this->that = that;
	this->manager = nullptr;
}
void AESpeechSynthesisBean::init(AEManager* mgr) {
	this->manager = mgr;
	static const char name[] = "app/engine/AESpeechSynthesisImpl";
	static const char sign[] = "(JLapp/engine/AEView;)V";
	AEJNI::jnew(name, sign, [this](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(this->manager);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}
void AESpeechSynthesisBean::release() {
	static const char name[] = "release";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}

void AESpeechSynthesisBean::pause() {
	static const char name[] = "pause";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESpeechSynthesisBean::speak(float rate, const char* lang_, const char* text_) {
	static const char name[] = "speak";
	static const char sign[] = "(Fjava/lang/String;java/lang/String;)V";
	AEJNI::call(impl, name, sign, [=](JNIEnv* jni, jobject obj, jmethodID method) {
		jstring lang = jni->NewStringUTF(lang_);
		jstring text = jni->NewStringUTF(text_);

		jni->CallVoidMethod(obj, method, rate, lang, text);

		jni->DeleteLocalRef(lang);
		jni->DeleteLocalRef(text);
	});
}
void AESpeechSynthesisBean::cancel() {
	static const char name[] = "cancel";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESpeechSynthesisBean::resume() {
	static const char name[] = "resume";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}
void AESpeechSynthesisBean::getVoices() {
	static const char name[] = "getVoices";
	static const char sign[] = "()V";
	AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
		jni->CallVoidMethod(obj, method);
	});
}

bool AESpeechSynthesisBean::paused() {
	bool ret = false;
	static const char name[] = "paused";
	static const char sign[] = "()Z";
	AEJNI::call(impl, name, sign, [&ret](JNIEnv* jni, jobject obj, jmethodID method) {
		ret = jni->CallBooleanMethod(obj, method);
	});
	return ret;
}
bool AESpeechSynthesisBean::pending() {
	bool ret = false;
	static const char name[] = "pending";
	static const char sign[] = "()Z";
	AEJNI::call(impl, name, sign, [&ret](JNIEnv* jni, jobject obj, jmethodID method) {
		ret = jni->CallBooleanMethod(obj, method);
	});
	return ret;
}
bool AESpeechSynthesisBean::speaking() {
	bool ret = false;
	static const char name[] = "speaking";
	static const char sign[] = "()Z";
	AEJNI::call(impl, name, sign, [&ret](JNIEnv* jni, jobject obj, jmethodID method) {
		ret = jni->CallBooleanMethod(obj, method);
	});
	return ret;
}

#endif
