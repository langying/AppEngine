#ifdef ANDROID

//
//  AEHTMLImageElementBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/9/28.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEFormat.hpp"
#include "AEManager.hpp"
#include "AEHTMLImageElement.hpp"
#include "AEHTMLImageElementBean.hpp"

std::map<AEHTMLImageElementBean*, int> AEHTMLImageElementBean::beans;
void AEHTMLImageElementBean::response(AEHTMLImageElementBean* bean, std::string src, GLuint width, GLuint height) {
    auto it = beans.find(bean);
    if (it == beans.end()) {
        return;
    }
    if (bean->src != src) {
        return;
    }
    beans.erase(bean);
    bean->img->onFinish(width, height);
}

AEHTMLImageElementBean::~AEHTMLImageElementBean() {
    auto it = beans.find(this);
    if (it != beans.end()) {
    	beans.erase(it);
    }
}
AEHTMLImageElementBean::AEHTMLImageElementBean(AEHTMLImageElement* image) {
    img = image;
    mgr = nullptr;
}
void AEHTMLImageElementBean::init(AEManager* mgr) {
	this->mgr = mgr;
}
void AEHTMLImageElementBean::setSrc(const char* url) {
    beans[this] = 0;
    src = mgr->absURL(url);

    static const char func[] = "onloadImage";
	static const char sign[] = "(JJLjava/lang/String;)V";
	AEJNI::view(mgr, func, sign, [this](JNIEnv* jni, jobject view, jmethodID method) {
		jstring url = jni->NewStringUTF(src.c_str());
		jni->CallVoidMethod(view, method, reinterpret_cast<jlong>(mgr), reinterpret_cast<jlong>(this), url);
		jni->DeleteLocalRef(url);
	});
}

void AEHTMLImageElementBean::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
    static const char func[] = "bitmapWithURL";
    static const char sign[] = "(Ljava/lang/String;)Landroid/graphics/Bitmap;";
	AEJNI::view(mgr, func, sign, [=](JNIEnv* jni, jobject view, jmethodID method) {
		jobject url = jni->NewStringUTF(src.c_str());
		jobject bmp = jni->CallObjectMethod(view, method, url);
		jni->DeleteLocalRef(url);
		do {
			if (bmp == nullptr) {
				LOG("bitmapWithURL returned bitmap is null.");
				break;
			}
			AndroidBitmapInfo info;
			int ret = AndroidBitmap_getInfo(jni, bmp, &info);
			if (ret < 0) {
				LOG("AndroidBitmap_getInfo() failed ! error=%d.", ret);
				break;
			}
			if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
				LOG("Bitmap format is not RGBA_8888.");
				break;
			}
			GLvoid* buffer = NULL;
			ret = AndroidBitmap_lockPixels(jni, bmp, &buffer);
			if (ret < 0) {
				LOG("AndroidBitmap_lockPixels() failed ! error=%d.", ret);
				break;
			}
			AEFormat::convert(buffer, info.width, info.height, flipY, alpha); // 这里可能会改变bitmap的底层pixels
			glTexImage2D(target, level, infmt, info.width, info.height, 0, format, type, buffer);
		} while(0);
		AndroidBitmap_unlockPixels(jni, bmp);
		jni->DeleteLocalRef(bmp);
	});
}
void AEHTMLImageElementBean::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
    static const char func[] = "bitmapWithURL";
    static const char sign[] = "(Ljava/lang/String;)Landroid/graphics/Bitmap;";
	AEJNI::view(mgr, func, sign, [=](JNIEnv* jni, jobject view, jmethodID method) {
		jobject url = jni->NewStringUTF(src.c_str());
		jobject bmp = jni->CallObjectMethod(view, method, url);
		jni->DeleteLocalRef(url);
		do {
			if (bmp == nullptr) {
				LOG("bitmapWithURL returned bitmap is null.");
				break;
			}
			AndroidBitmapInfo info;
			int ret = AndroidBitmap_getInfo(jni, bmp, &info);
			if (ret < 0) {
				LOG("AndroidBitmap_getInfo() failed ! error=%d.", ret);
				break;
			}
			if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
				LOG("Bitmap format is not RGBA_8888.");
				break;
			}
			GLvoid* buffer = NULL;
			ret = AndroidBitmap_lockPixels(jni, bmp, &buffer);
			if (ret < 0) {
				LOG("AndroidBitmap_lockPixels() failed ! error=%d.", ret);
				break;
			}
			AEFormat::convert(buffer, info.width, info.height, flipY, alpha); // 这里可能会改变bitmap的底层pixels
			glTexSubImage2D(target, level, xoffset, yoffset, info.width, info.height, format, type, buffer);
		} while(0);
		AndroidBitmap_unlockPixels(jni, bmp);
		jni->DeleteLocalRef(bmp);
	});
}
#endif
