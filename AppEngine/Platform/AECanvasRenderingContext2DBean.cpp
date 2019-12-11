#ifdef ANDROID
//
//  AECanvasRenderingContext2DBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEJNI.hpp"
#include "AEFormat.hpp"
#include "AEHTMLImageElement.hpp"
#include "AECanvasRenderingContext2DBean.hpp"

AECanvasRenderingContext2DBean::~AECanvasRenderingContext2DBean() {
    AEJNI::jni()->DeleteGlobalRef(impl);
    LOG("delete bitmap[%dx%d][%d KB].", _width, _height, _width * _height * 4 / 1024);
}
AECanvasRenderingContext2DBean::AECanvasRenderingContext2DBean() {
	impl       = nullptr;
    _width     = 300;
    _height    = 150;
    _fillColor = 0;
    _fontColor = 0;
    _fontSize  = 16;
    _lineWidth = 1;
    _fontName  = "";
}
void AECanvasRenderingContext2DBean::setWidth(GLuint val) {
	_width = val;
}
void AECanvasRenderingContext2DBean::setHeight(GLuint val) {
	_height = val;
}
void AECanvasRenderingContext2DBean::setFillColor(GLuint val) {
    _fillColor = val;
    static const char func[] = "setFillColor";
    static const char sign[] = "(I)V";
    AEJNI::call(impl, func, sign, [val](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, val);
    });
}
void AECanvasRenderingContext2DBean::setFontColor(GLuint val) {
    _fontColor = val;
    static const char func[] = "setFontColor";
    static const char sign[] = "(I)V";
    AEJNI::call(impl, func, sign, [val](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, val);
    });
}
void AECanvasRenderingContext2DBean::setFontSize(GLfloat val) {
    _fontSize = val;
    static const char func[] = "setFontSize";
    static const char sign[] = "(F)V";
    AEJNI::call(impl, func, sign, [val](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, val);
    });
}
void AECanvasRenderingContext2DBean::setLineWidth(GLfloat val) {
    _lineWidth = val;
    static const char func[] = "setLineWidth";
    static const char sign[] = "(F)V";
    AEJNI::call(impl, func, sign, [val](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, val);
    });
}
void AECanvasRenderingContext2DBean::setFontName(GLstring val_) {
	_fontName = val_;
    static const char func[] = "setFontName";
    static const char sign[] = "(Ljava/lang/String;)V";
    AEJNI::call(impl, func, sign, [val_](JNIEnv* jni, jobject canvas, jmethodID method) {
    	jstring val = jni->NewStringUTF(val_.c_str());
        jni->CallVoidMethod(canvas, method, val);
        jni->DeleteLocalRef(val);
    });
}
void AECanvasRenderingContext2DBean::setFillStyle(GLstring val_) {
	_fillStyle = val_;
    static const char func[] = "setFillStyle";
    static const char sign[] = "(Ljava/lang/String;)V";
    AEJNI::call(impl, func, sign, [val_](JNIEnv* jni, jobject canvas, jmethodID method) {
    	jstring val = jni->NewStringUTF(val_.c_str());
        jni->CallVoidMethod(canvas, method, val);
        jni->DeleteLocalRef(val);
    });
}

void AECanvasRenderingContext2DBean::init(AEManager* mgr) {
    static const char name[] = "app/engine/AECanvasRenderingContext2DImpl";
    static const char sign[] = "(Lapp/engine/AEView;II)V";
	AEJNI::jnew(name, sign, [this, mgr](JNIEnv* jni, jclass clazz, jmethodID method) {
		jobject view = AEJNI::viewWithMgr(mgr);
		jobject jret = jni->NewObject(clazz, method, view, _width, _height);
		this->impl = jni->NewGlobalRef(jret);
		jni->DeleteLocalRef(jret);
	});
}
void AECanvasRenderingContext2DBean::resize(int width, int height) {
    _width  = width;
    _height = height;
    static const char func[] = "resize";
    static const char sign[] = "(II)V";
    AEJNI::call(impl, func, sign, [width, height](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, width, height);
    });
}
void AECanvasRenderingContext2DBean::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
    static const char func[] = "bitmap";
    static const char sign[] = "()Landroid/graphics/Bitmap;";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jobject bmp = jni->CallObjectMethod(canvas, method);
        do {
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
            GLvoid* buffer = nullptr;
            ret = AndroidBitmap_lockPixels(jni, bmp, &buffer);
            if (ret < 0) {
                LOG("AndroidBitmap_lockPixels() failed ! error=%d.", ret);
                break;
            }
            AEFormat::convert(buffer, info.width, info.height, flipY, alpha); // 这里可能会改变bitmap的底层pixels
            glTexImage2D(target, level, infmt, info.width, info.height, 0, format, type, buffer);
            AndroidBitmap_unlockPixels(jni, bmp);
        } while(0);
        jni->DeleteLocalRef(bmp);
    });
}
void AECanvasRenderingContext2DBean::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
    static const char func[] = "bitmap";
    static const char sign[] = "()Landroid/graphics/Bitmap;";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jobject bmp = jni->CallObjectMethod(canvas, method);
        do {
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
            GLvoid* buffer = nullptr;
            ret = AndroidBitmap_lockPixels(jni, bmp, &buffer);
            if (ret < 0) {
                LOG("AndroidBitmap_lockPixels() failed ! error=%d.", ret);
                break;
            }
            AEFormat::convert(buffer, info.width, info.height, flipY, alpha); // 这里可能会改变bitmap的底层pixels
            glTexSubImage2D(target, level, xoffset, yoffset, info.width, info.height, format, type, buffer);
            AndroidBitmap_unlockPixels(jni, bmp);
        } while(0);
        jni->DeleteLocalRef(bmp);
    });
}

void AECanvasRenderingContext2DBean::clearRect(int x, int y, int w, int h) {
    static const char func[] = "clearRect";
    static const char sign[] = "(IIII)V";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, x, y, w, h);
    });
}
void AECanvasRenderingContext2DBean::drawImage(AEImage* img, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh) {
    static const char func[] = "drawImage";
    static const char sign[] = "(Ljava/lang/String;IIIIIIII)V";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jstring url = jni->NewStringUTF(img->pathfile().c_str());
        jni->CallVoidMethod(canvas, method, url, sx, sy, sw, sh, dx, dy, dw, dh);
        jni->DeleteLocalRef(url);
    });
}
void AECanvasRenderingContext2DBean::fillRect(int x, int y, int w, int h) {
    static const char func[] = "fillRect";
    static const char sign[] = "(IIII)V";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, x, y, w, h);
    });
}
void AECanvasRenderingContext2DBean::fillText(const char* text_, int x, int y) {
    static const char func[] = "fillText";
    static const char sign[] = "(Ljava/lang/String;II)V";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jstring text = jni->NewStringUTF(text_ ? : "");
        jni->CallVoidMethod(canvas, method, text, x, y);
        jni->DeleteLocalRef(text);
    });
}
void AECanvasRenderingContext2DBean::getImageData(int x, int y, int w, int h, void* pixels) {
    static const char func[] = "bitmap";
    static const char sign[] = "()Landroid/graphics/Bitmap;";
    AEJNI::call(impl, func, sign, [pixels](JNIEnv* jni, jobject canvas, jmethodID method) {
        jobject bitmap = jni->CallObjectMethod(canvas, method);
        do {
            AndroidBitmapInfo info;
            int ret = AndroidBitmap_getInfo(jni, bitmap, &info);
            if (ret < 0) {
                LOG("AndroidBitmap_getInfo() failed ! error=%d.", ret);
                break;
            }
            if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
                LOG("Bitmap format is not RGBA_8888.");
                break;
            }
            GLvoid* buffer = nullptr;
            ret = AndroidBitmap_lockPixels(jni, bitmap, &buffer);
            if (ret < 0) {
                LOG("AndroidBitmap_lockPixels() failed ! error=%d.", ret);
                break;
            }
            memcpy(pixels, buffer, info.width * info.height * 4);
            AndroidBitmap_unlockPixels(jni, bitmap);
        } while(0);
        jni->DeleteLocalRef(bitmap);
    });
}
float AECanvasRenderingContext2DBean::measureText(const char* text_) {
    float length = 0;
    static const char func[] = "measureText";
    static const char sign[] = "(Ljava/lang/String;)F";
    AEJNI::call(impl, func, sign, [&](JNIEnv* jni, jobject canvas, jmethodID method) {
        jstring text = jni->NewStringUTF(text_ ? : "");
        length = jni->CallFloatMethod(canvas, method, text);
        jni->DeleteLocalRef(text);
    });
    return length;
}
void AECanvasRenderingContext2DBean::scale(float x, float y) {
    static const char func[] = "scale";
    static const char sign[] = "(FF)V";
    AEJNI::call(impl, func, sign, [x, y](JNIEnv* jni, jobject canvas, jmethodID method) {
        jni->CallVoidMethod(canvas, method, x, y);
    });
}
void AECanvasRenderingContext2DBean::strokeText(const char* text_, int x, int y) {
    static const char func[] = "strokeText";
    static const char sign[] = "(Ljava/lang/String;II)V";
    AEJNI::call(impl, func, sign, [=](JNIEnv* jni, jobject canvas, jmethodID method) {
        jstring text = jni->NewStringUTF(text_ ? : "");
        jni->CallVoidMethod(canvas, method, text, x, y);
        jni->DeleteLocalRef(text);
    });
}

#endif
