#ifdef ANDROID
//
//  AEVRDisplayBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/3.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEManager.hpp"
#include "AEVRDisplay.hpp"
#include "AEVRDisplayBean.hpp"

AEVRDisplayBean::~AEVRDisplayBean() {
    stop();
    AEJNI::jni()->DeleteGlobalRef(impl);
}
AEVRDisplayBean::AEVRDisplayBean(AEVRDisplay* display) {
	this->impl    = nullptr;
	this->manager = nullptr;
    this->display = display;
}
void AEVRDisplayBean::init(AEManager* mgr) {
	this->manager = mgr;
	static const char name[] = "app/engine/AEVRDisplayImpl";
	static const char sign[] = "(JLapp/engine/AEView;)V";
	AEJNI::jnew(name, sign, [this](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(this->manager);
		jobject jret = jni->NewObject(clazz, method, reinterpret_cast<jlong>(this), view);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}
void AEVRDisplayBean::stop() {
    static const char name[] = "stopTracking";
    static const char sign[] = "()V";
    AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
        jni->CallVoidMethod(obj, method);
    });
}
void AEVRDisplayBean::start() {
    static const char name[] = "startTracking";
    static const char sign[] = "()V";
    AEJNI::call(impl, name, sign, [](JNIEnv* jni, jobject obj, jmethodID method) {
        jni->CallVoidMethod(obj, method);
    });
}
void AEVRDisplayBean::update() {
    static const char name[] = "toWebVRArray";
    static const char sign[] = "()[F";
    AEJNI::call(impl, name, sign, [this](JNIEnv* jni, jobject obj, jmethodID method) {
        jfloatArray list = (jfloatArray)jni->CallObjectMethod(obj, method, 0);
        jfloat* data = jni->GetFloatArrayElements(list, 0);
        display->x  = data[0];
        display->y  = data[1];
        display->z  = data[2];
        display->w  = data[3];
        display->lu = data[4];
        display->ld = data[5];
        display->ll = data[6];
        display->lr = data[7];
        display->ru = data[8];
        display->rd = data[9];
        display->rl = data[10];
        display->rr = data[11];
        jni->ReleaseFloatArrayElements(list, data, 0);
        jni->DeleteLocalRef(list);
    });
}

#endif
