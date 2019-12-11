#ifdef ANDROID
//
//  AEJNI.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEJNI_hpp
#define AEJNI_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEJNI {
public:
	static JavaVM* jvm;
	static std::map<void*, jobject> views;

	static JNIEnv* jni();
	static jobject viewWithMgr(AEManager* mgr);
	static jobject viewWithCtx(duk_context* ctx);
	static AEManager* delMapping(AEManager* mgr);
	static AEManager* addMapping(AEManager* mgr, jobject view);

public:
	template<typename Handle>
	static inline GLvoid call(const char* name, const char* func, const char* sign, Handle handle) {
		do {
			JNIEnv *jni = AEJNI::jni();
			jclass clazz = jni->FindClass(name);
			jmethodID method = jni->GetStaticMethodID(clazz, func, sign);
			handle(jni, clazz, method);
			jni->DeleteLocalRef(clazz);
		}
		while (0);
	}

	template<typename Handle>
	static inline GLvoid call(jobject bean, const char* func, const char* sign, Handle handle) {
		do {
			if (!bean) {
				break;
			}
			JNIEnv *jni = AEJNI::jni();
			jclass clazz = jni->GetObjectClass(bean);
			jmethodID method = jni->GetMethodID(clazz, func, sign);
			if (!method) {
				LOG("MethodNotFound[%s%s]", func, sign);
				break;
			}
			handle(jni, bean, method);
			jni->DeleteLocalRef(clazz);
		}
		while (0);
	}

	template<typename Handle>
	static inline GLvoid jnew(const char* name, const char* sign, Handle handle) {
		do {
			JNIEnv *jni = AEJNI::jni();
			jclass clazz = jni->FindClass(name);
			if (!clazz) {
				LOG("ClassNotFound[%s]", name);
				break;
			}
			jmethodID method = jni->GetMethodID(clazz, "<init>", sign);
			if (!method) {
				LOG("MethodNotFound[%s.init%s]", name, sign);
				break;
			}
			handle(jni, clazz, method);
			jni->DeleteLocalRef(clazz);
		}
		while (0);
	}

	template<typename Handle>
	static inline GLvoid view(AEManager* mgr, const char* func, const char* sign, Handle handle) {
		do {
			JNIEnv* jni = AEJNI::jni();
			jobject view = viewWithMgr(mgr);
			if (!view) {
				LOGE("GetGLView(%p) is null.", mgr);
				break;
			}
			jclass clazz = jni->GetObjectClass(view);
			jmethodID method = jni->GetMethodID(clazz, func, sign);
			if (!method) {
				LOG("MethodNotFound[%s%s]", func, sign);
				break;
			}
			handle(jni, view, method);
			jni->DeleteLocalRef(clazz);
		}
		while (0);
	}
};

#endif /* AEJNI_hpp */
#endif /* ANDROID */
