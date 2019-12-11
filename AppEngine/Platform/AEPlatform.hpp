//
//  AEPlatform.hpp
//  Action
//
//  Created by 韩琼 on 15/12/28.
//  Copyright © 2015年 AppEngine. All rights reserved.
//

#ifndef AEPlatform_hpp
#define AEPlatform_hpp

#include <map>
#include <string>
#include <vector>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>

#include "duktape.h"
#include "AEBase.hpp"

#ifdef ANDROID
#   include <jni.h>
#   include <functional>
#   include <GLES2/gl2.h>
#   include <GLES2/gl2ext.h>
#   include <android/log.h>
#   include <android/bitmap.h>

#   define __FILE_NAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#   define LOG( fmt, ...); __android_log_print(ANDROID_LOG_INFO,  "AppEngine", "[%s %d %s]" fmt, __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGD(fmt, ...); __android_log_print(ANDROID_LOG_DEBUG, "AppEngine", "[%s %d %s]" fmt, __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGI(fmt, ...); __android_log_print(ANDROID_LOG_INFO,  "AppEngine", "[%s %d %s]" fmt, __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGW(fmt, ...); __android_log_print(ANDROID_LOG_WARN,  "AppEngine", "[%s %d %s]" fmt, __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGE(fmt, ...); __android_log_print(ANDROID_LOG_ERROR, "AppEngine", "[%s %d %s]" fmt, __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);

#   define GL_RGB16F_EXT               0x881B
#   define GL_RGBA16F_EXT              0x881A
#   define GL_RGB32F_EXT               0x8815
#   define GL_RGBA32F_EXT              0x8814
#   define GL_BGRA                     GL_BGRA_EXT
#   define GL_SRGB_EXT                 0x8C40
#   define GL_SRGB_ALPHA_EXT           0x8C42
#   define GL_ALPHA16F_EXT             0x881C
#   define GL_LUMINANCE16F_EXT         0x881E
#   define GL_LUMINANCE_ALPHA32F_EXT   0x8819
#   define GL_ALPHA32F_EXT             0x8816
#   define GL_LUMINANCE32F_EXT         0x8818
#   define GL_LUMINANCE_ALPHA16F_EXT   0x881F
#else
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>
#   include <TargetConditionals.h>

#   define __FILE_NAME__ ((strrchr(__FILE__, '/') ?: __FILE__ - 1) + 1)
#   define LOG( fmt, ...); printf("[%s %d %s]"   fmt "\n", __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGD(fmt, ...); printf("[D %s %d %s]" fmt "\n", __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGI(fmt, ...); printf("[I %s %d %s]" fmt "\n", __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGW(fmt, ...); printf("[W %s %d %s]" fmt "\n", __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);
#   define LOGE(fmt, ...); printf("[E %s %d %s]" fmt "\n", __FILE_NAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);

typedef void* jobject;
#endif

/** WebGL-specific enums */
#define GL_BGR                                  0x80E0
#define GL_DEPTH_STENCIL                        GL_DEPTH_STENCIL_OES
#define GL_DEPTH_STENCIL_ATTACHMENT             0x821A
#define GL_UNPACK_FLIP_Y_WEBGL                  0x9240
#define GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL       0x9241
#define GL_CONTEXT_LOST_WEBGL                   0x9242
#define GL_UNPACK_COLORSPACE_CONVERSION_WEBGL   0x9243
#define GL_BROWSER_DEFAULT_WEBGL                0x9244
#define GL_UNMASKED_VENDOR_WEBGL				0x9245
#define GL_UNMASKED_RENDERER_WEBGL				0x9246

#define duk_get_bool    duk_get_boolean
#define duk_get_float   duk_get_number
#define duk_get_double  duk_get_number
#define duk_push_bool   duk_push_boolean
#define duk_push_float  duk_push_number
#define duk_push_double duk_push_number

#define ae_free(ptr); \
if (ptr) { free(ptr); ptr = nullptr; }

#define ae_prop(type, Name, name); \
GL##type _##name; \
GL##type get##Name() { \
    return _##name;\
} \
GLvoid set##Name(GL##type value) { \
    _##name = value;\
}\

#define js_set_int(name, value); \
duk_push_int(ctx, value); \
duk_put_prop_string(ctx, -2, name);

#define js_set_pptr(name, value); \
duk_push_pointer(ctx, value); \
duk_put_prop_string(ctx, -2, name);

#define js_set_bool(name, value); \
duk_push_boolean(ctx, value); \
duk_put_prop_string(ctx, -2, name);

#define js_set_uint(name, value); \
duk_push_uint(ctx, value); \
duk_put_prop_string(ctx, -2, name);

#define js_set_numb(name, value); \
duk_push_number(ctx, value); \
duk_put_prop_string(ctx, -2, name);

#define js_set_strs(name, value); \
duk_push_string(ctx, value); \
duk_put_prop_string(ctx, -2, name);

#define js_set_func(argc, name, func); \
duk_push_c_function(ctx, func, argc); \
duk_put_prop_string(ctx, -2, name);

#define js_func(name); \
static int name(duk_context* ctx);

#define js_define(name); \
static int get_##name(duk_context* ctx); \
static int set_##name(duk_context* ctx);

#define js_thiz(clazz); \
duk_push_this(ctx); \
duk_get_prop_string(ctx, -1, js_tag); \
clazz* thiz = static_cast<clazz*>(duk_get_pointer(ctx, -1)); \
duk_pop_2(ctx);

#define js_that(idx, clazz, name);\
duk_get_prop_string(ctx, idx, js_tag); \
clazz* name = static_cast<clazz*>(duk_get_pointer(ctx, -1)); \
duk_pop(ctx);

#define js_begin_binding(); \
duk_push_global_object(ctx); \
duk_push_c_function(ctx, constructor, DUK_VARARGS); \
duk_push_object(ctx); \
duk_dup(ctx, -2); \
duk_put_prop_string(ctx, -2, "constructor");

#define js_add_prop(name); \
duk_push_string(ctx, #name);\
duk_push_c_function(ctx, get_##name, 0);\
duk_push_c_function(ctx, set_##name, 1);\
duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

#define js_add_alias(name, alias);\
duk_push_string(ctx, #alias);\
duk_push_c_function(ctx, get_##name, 0);\
duk_push_c_function(ctx, set_##name, 1);\
duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);

#define js_add_value(type, value, name);\
duk_push_string(ctx, #name);	\
duk_push_##type(ctx, value);\
duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_ENUMERABLE);

#define js_binding_func(argc, name); \
duk_push_c_function(ctx, name, argc); \
duk_put_prop_string(ctx, -2, #name);

#define js_end_binding(name) \
duk_put_prop_string(ctx, -2, "prototype");\
duk_put_prop_string(ctx, -2, #name);\
duk_pop(ctx);

#define js_prop(clazz, type, name); \
GL##type name; \
GL##type get_##name() { \
    return name;\
} \
GLvoid set_##name(GL##type value) { \
    name = value;\
}\
static int get_##name(duk_context *ctx) { \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, js_tag); \
    clazz* thiz = static_cast<clazz*>(duk_get_pointer(ctx, -1)); \
    duk_pop_2(ctx); \
    duk_push_##type(ctx, thiz->name); \
    return 1; \
} \
static int set_##name(duk_context *ctx) { \
    duk_push_this(ctx); \
    duk_get_prop_string(ctx, -1, js_tag); \
    clazz* thiz = static_cast<clazz*>(duk_get_pointer(ctx, -1)); \
    thiz->name = duk_get_##type(ctx, 0); \
    duk_pop_2(ctx); \
    return 0; \
}

//finalizer参数1个，就是将被gc回收的js对象
//constructor模式调用的使用stack上不用push返回值
#define js_con_destructor(clazz, handle); \
duk_context* cx;\
~clazz();\
clazz();\
clazz(duk_context* ctx): clazz() {\
    cx = ctx;\
};\
static void binding(duk_context* ctx);\
static int destructor(duk_context* ctx) { \
    duk_get_prop_string(ctx, 0, js_tag); \
    delete static_cast<clazz*>(duk_get_pointer(ctx, -1)); \
    duk_pop(ctx); \
    return 0; \
} \
static int constructor(duk_context* ctx) { \
    clazz* thiz = new clazz(ctx);\
    duk_push_this(ctx); \
    duk_push_pointer(ctx, thiz); \
    duk_put_prop_string(ctx, -2, js_tag); \
    duk_push_c_function(ctx, destructor, 1); \
    duk_set_finalizer(ctx, -2); \
    duk_pop(ctx); \
    handle(ctx, thiz, duk_get_top(ctx)); \
    return 0; \
}


/**
 * http://www.opengl.org/wiki/OpenGL_Error
 * 0x0500, GL_INVALID_ENUM
 * 0x0501, GL_INVALID_VALUE
 * 0x0502, GL_INVALID_OPERATION
 * 0x0503, GL_STACK_OVERFLOW
 * 0x0504, GL_STACK_UNDERFLOW
 * 0x0505, GL_OUT_OF_MEMORY
 * 0x0506, GL_INVALID_FRAMEBUFFER_OPERATION
 * 0x8031, GL_TABLE_TOO_LARGE​1
 */
#define GLCheckError();                                         \
for (GLenum error = glGetError(); error; error = glGetError()) {\
    LOG("glError:0x%x\n", error);                               \
}

static const char js_tag[] = "_";
typedef double      GLdouble;
typedef GLboolean   GLbool;
typedef std::string GLstring;
static inline const char* duk_push_string(duk_context* ctx, std::string& string) {
	return duk_push_string(ctx, string.c_str());
}

class AEManager;
class AEPlatform {
public:
	static GLuint ID;
	static GLuint UUID();

public:
	static void  alert(AEManager* mgr, const char* text);
	static void  speak(AEManager* mgr, const char* text, const char* lang, float rate);
	static void  delItem(AEManager* mgr, const char* key);
	static char* getItem(AEManager* mgr, const char* key);
	static void  setItem(AEManager* mgr, const char* key, const char* value);
	static char* textWithURL(AEManager* mgr, const char* url);

public:
    static std::string encodeURL(const char* url);
    static std::string decodeURL(const char* url);
};

class AEImage {
public:
    virtual ~AEImage() {};
    virtual std::string pathfile() { return ""; }
	virtual GLuint get_width() = 0;
	virtual GLuint get_height() = 0;
    virtual GLvoid texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha) = 0;
    virtual GLvoid texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha) = 0;
};

#endif /* AEPlatform_hpp */
