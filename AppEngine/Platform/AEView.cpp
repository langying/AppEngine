#ifdef ANDROID
//
//  Amaze.cpp
//  Action
//
//  Created by 韩琼 on 15/12/28.
//  Copyright © 2015年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEManager.hpp"
#include "AEHTMLImageElementBean.hpp"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
	AEJNI::jvm = vm;
	return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL
JNI_OnUnload(JavaVM* vm, void* reserved) {
}

JNIEXPORT jlong JNICALL
Java_app_engine_AEView_create(JNIEnv* jni, jobject view, jint width, jint height, jfloat scale, jstring agent_) {
	const char* agent = jni->GetStringUTFChars(agent_, 0);
	AEManager* mgr = new AEManager(width, height, scale, agent);
	AEJNI::addMapping(mgr, view)->init();
	jni->ReleaseStringUTFChars(agent_, agent);
	return reinterpret_cast<jlong>(mgr);
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_jsgc(JNIEnv* jni, jobject view_, jlong mgr_) {
	reinterpret_cast<AEManager*>(mgr_)->jsgc();
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_delete(JNIEnv* jni, jobject view_, jlong mgr_) {
	AEManager* mgr = reinterpret_cast<AEManager*>(mgr_);
	delete AEJNI::delMapping(mgr);
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_loadURL(JNIEnv* jni, jobject view, jlong mgr_, jstring url_) {
	AEManager*  mgr = reinterpret_cast<AEManager*>(mgr_);
	const char* url = jni->GetStringUTFChars(url_, 0);
	mgr->loadURL(url);
	jni->ReleaseStringUTFChars(url_, url);
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_applyTimer(JNIEnv* jni, jobject view, jlong mgr_) {
	reinterpret_cast<AEManager*>(mgr_)->applyTimer();
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_applyResize(JNIEnv* jni, jobject view, jlong mgr, jint width, jint height) {
	reinterpret_cast<AEManager*>(mgr)->applyResize(width, height);
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_applyKeyboard(JNIEnv* jni, jobject view_, jlong mgr, jint type, jint keyCode) {
	reinterpret_cast<AEManager*>(mgr)->applyKeyboard(static_cast<enum AEEvent::Type>(type), keyCode);
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_touch(JNIEnv* env, jobject view_, jlong mgr_, jint type, jlongArray ids_, jfloatArray pxs_, jfloatArray pys_) {
    jsize   len = env->GetArrayLength(ids_);
	jlong*  ids = env->GetLongArrayElements( ids_, NULL);
	jfloat* pxs = env->GetFloatArrayElements(pxs_, NULL);
	jfloat* pys = env->GetFloatArrayElements(pys_, NULL);

	AETouch* pts = (AETouch*)malloc(len * sizeof(AETouch));
	for (int idx = 0; idx < len; idx++) {
		pts[idx].ID = ids[idx];
		pts[idx].x  = pxs[idx];
		pts[idx].y  = pys[idx];
	}
	reinterpret_cast<AEManager*>(mgr_)->touch(static_cast<enum AEEvent::Type>(type), len, pts);
	free(pts);

	env->ReleaseLongArrayElements( ids_, ids, 0);
	env->ReleaseFloatArrayElements(pxs_, pxs, 0);
	env->ReleaseFloatArrayElements(pys_, pys, 0);
}

JNIEXPORT jstring JNICALL
Java_app_engine_AEView_evalute(JNIEnv* jni, jobject view_, jlong mgr, jstring script_) {
	const char* script = jni->GetStringUTFChars(script_, 0);
	std::string result = reinterpret_cast<AEManager*>(mgr)->evalute(script);
	jni->ReleaseStringUTFChars(script_, script);
	return jni->NewStringUTF(result.c_str());
}

JNIEXPORT void JNICALL
Java_app_engine_AEView_finishImage(JNIEnv* jni, jobject view, jlong mgr_, jlong img_, jstring src_, jint width, jint height) {
	const char* src = jni->GetStringUTFChars(src_, 0);
	AEHTMLImageElementBean::response(reinterpret_cast<AEHTMLImageElementBean*>(img_), src, width, height);
	jni->ReleaseStringUTFChars(src_, src);
}

#ifdef __cplusplus
}
#endif

#endif
