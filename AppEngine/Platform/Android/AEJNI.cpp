#ifdef ANDROID
//
//  AEJNI.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEManager.hpp"

JavaVM* AEJNI::jvm = nullptr;
std::map<void*, jobject> AEJNI::views;

JNIEnv* AEJNI::jni() {
	JNIEnv* env = nullptr;
	jvm->GetEnv((void**) &env, JNI_VERSION_1_6);
	return env;
}
jobject AEJNI::viewWithMgr(AEManager* mgr) {
	auto it = views.find(mgr);
	return it == views.end() ? nullptr : it->second;
}
jobject AEJNI::viewWithCtx(duk_context* ctx) {
	AEManager* mgr = AEManager::find(ctx);
	return viewWithMgr(mgr);
}
AEManager* AEJNI::addMapping(AEManager* mgr, jobject view) {
	views[mgr] = jni()->NewGlobalRef(view);
	return mgr;
}
AEManager* AEJNI::delMapping(AEManager* mgr) {
	auto it = views.find(mgr);
	if (it != views.end()) {
		jni()->DeleteGlobalRef(it->second);
		views.erase(it);
	}
	return mgr;
}

#endif
