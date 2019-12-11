#ifdef ANDROID
//
//  AEPlatform.cpp
//  Action
//
//  Created by 韩琼 on 15/12/28.
//  Copyright © 2015年 AppEngine. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEFormat.hpp"
#include "AEManager.hpp"
#include "AEPlatform.hpp"
#include "AEHTMLImageElement.hpp"

GLuint AEPlatform::ID = 0;
GLuint AEPlatform::UUID() {
    return ID++;
}

void AEPlatform::alert(AEManager* mgr, const char* text_) {
    static const char func[] = "alert";
    static const char sign[] = "(Ljava/lang/String;)V";
    AEJNI::view(mgr, func, sign, [text_](JNIEnv* jni, jobject view, jmethodID method) {
        jstring text = jni->NewStringUTF(text_);
        jni->CallVoidMethod(view, method, text);
        jni->DeleteLocalRef(text);
    });
}

void AEPlatform::speak(AEManager* mgr, const char* text_, const char* lang_, float rate) {
    static const char func[] = "speak";
    static const char sign[] = "(Ljava/lang/String;Ljava/lang/String;F)V";
    AEJNI::view(mgr, func, sign, [text_, lang_, rate](JNIEnv* jni, jobject view, jmethodID method) {
        jstring text = jni->NewStringUTF(text_);
        jstring lang = jni->NewStringUTF(lang_);
        jni->CallVoidMethod(view, method, text, lang, rate);
        jni->DeleteLocalRef(text);
        jni->DeleteLocalRef(lang);
    });
}

void AEPlatform::delItem(AEManager* mgr, const char* key_) {
    static const char func[] = "delItem";
    static const char sign[] = "(Ljava/lang/String;)V";
    AEJNI::view(mgr, func, sign, [key_](JNIEnv* jni, jobject view, jmethodID method) {
        jstring key = jni->NewStringUTF(key_);
        jni->CallVoidMethod(view, method, key);
        jni->DeleteLocalRef(key);
    });
}

char* AEPlatform::getItem(AEManager* mgr, const char* name_) {
    static const char func[] = "getItem";
    static const char sign[] = "(Ljava/lang/String;)Ljava/lang/String;";
    GLchar* text = nullptr;
    AEJNI::view(mgr, func, sign, [&text, name_](JNIEnv* jni, jobject view, jmethodID method) {
        jstring name = jni->NewStringUTF(name_);
        jstring jstr = (jstring)jni->CallObjectMethod(view, method, name);
        jni->DeleteLocalRef(name);
        
        if (jstr == nullptr) {
            LOG("getItem(%s) return null.", name_);
            return;
        }
        const GLint size = jni->GetStringUTFLength(jstr);
        const char* buff = jni->GetStringUTFChars(jstr, nullptr);
        text = (char*)calloc(size + 1, sizeof(char));
        memcpy(text, buff, size);
        jni->ReleaseStringUTFChars(jstr, buff);
    });
    return text;
}

void AEPlatform::setItem(AEManager* mgr, const char* key_, const char* val_) {
    static const char func[] = "setItem";
    static const char sign[] = "(Ljava/lang/String;Ljava/lang/String;)V";
    AEJNI::view(mgr, func, sign, [=](JNIEnv* jni, jobject view, jmethodID method) {
        jstring key = jni->NewStringUTF(key_);
        jstring val = jni->NewStringUTF(val_);
        jni->CallVoidMethod(view, method, key, val);
        jni->DeleteLocalRef(key);
        jni->DeleteLocalRef(val);
    });
}

char* AEPlatform::textWithURL(AEManager* mgr, const char* url_) {
    static const char func[] = "textWithURL";
    static const char sign[] = "(Ljava/lang/String;)Ljava/lang/String;";
    void* text = nullptr;
    AEJNI::view(mgr, func, sign, [&](JNIEnv* jni, jobject view, jmethodID method) {
        jstring url = jni->NewStringUTF(url_);
        jstring str = (jstring)jni->CallObjectMethod(view, method, url);
        jni->DeleteLocalRef(url);
        
        const GLuint  size = jni->GetStringUTFLength(str);
        const GLchar* buff = jni->GetStringUTFChars(str, 0);
        text = calloc(size + 1, sizeof(char));
        memcpy(text, buff, size);
        jni->ReleaseStringUTFChars(str, buff);
    });
    return static_cast<char*>(text);
}

std::string AEPlatform::encodeURL(const char* url_) {
	static const char name[] = "app/engine/AEPlatform";
    static const char func[] = "encodeURL";
    static const char sign[] = "(Ljava/lang/String;)Ljava/lang/String;";
    std::string ret;
    AEJNI::call(name, func, sign, [&](JNIEnv* jni, jclass clazz, jmethodID method) {
        jstring url = jni->NewStringUTF(url_);
        jstring str = (jstring)jni->CallStaticObjectMethod(clazz, method, url);
        jni->DeleteLocalRef(url);

        const char* str_ = jni->GetStringUTFChars(str, 0);
        ret = str_;
        jni->ReleaseStringUTFChars(str, str_);
    });
    return ret;
}

std::string AEPlatform::decodeURL(const char* url_) {
	static const char name[] = "app/engine/AEPlatform";
    static const char func[] = "decodeURL";
    static const char sign[] = "(Ljava/lang/String;)Ljava/lang/String;";
    std::string ret;
    AEJNI::call(name, func, sign, [&](JNIEnv* jni, jclass clazz, jmethodID method) {
        jstring url = jni->NewStringUTF(url_);
        jstring str = (jstring)jni->CallStaticObjectMethod(clazz, method, url);
        jni->DeleteLocalRef(url);

        const char* str_ = jni->GetStringUTFChars(str, 0);
        ret = str_;
        jni->ReleaseStringUTFChars(str, str_);
    });
    return ret;
}

#endif
