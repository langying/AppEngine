//
//  AEManager.cpp
//  Action
//
//  Created by 韩琼 on 16/1/9.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AESound.hpp"
#include "AEManager.hpp"
#include "AEImageData.hpp"
#include "AEVRDisplay.hpp"
#include "AEWebSocket.hpp"
#include "AEXMLHttpRequest.hpp"
#include "AESpeechSynthesis.hpp"
#include "AEHTMLAudioElement.hpp"
#include "AEHTMLImageElement.hpp"
#include "AEHTMLVideoElement.hpp"
#include "AEWebGLRenderingContext.hpp"
#include "AECanvasRenderingContext2D.hpp"

#include "AEMath.hpp"


char AEManager::KeyUp[]       = "keyup";
char AEManager::KeyDown[]     = "keydown";
char AEManager::MouseUp[]     = "mouseup";
char AEManager::MouseOut[]    = "mouseout";
char AEManager::MouseDown[]   = "mousedown";
char AEManager::MouseMove[]   = "mousemove";
char AEManager::TouchEnd[]    = "touchend";
char AEManager::TouchMove[]   = "touchmove";
char AEManager::TouchStart[]  = "touchstart";
char AEManager::TouchCancel[] = "touchcancel";

char* AEManager::event(AEEvent::Type type) {
    switch (type) {
        case AEEvent::KeyUp:
            return KeyUp;
        case AEEvent::KeyDown:
            return KeyDown;
        case AEEvent::MouseUp:
            return MouseUp;
        case AEEvent::MouseOut:
            return MouseOut;
        case AEEvent::MouseDown:
            return MouseDown;
        case AEEvent::MouseMove:
            return MouseMove;
        case AEEvent::TouchEnd:
            return TouchEnd;
        case AEEvent::TouchMove:
            return TouchMove;
        case AEEvent::TouchStart:
            return TouchStart;
        case AEEvent::TouchCancel:
            return TouchCancel;
    }
    return nullptr;
}

AEManager* AEManager::find(duk_context* ctx) {
    duk_push_global_object(ctx);
    duk_get_prop_string(ctx, -1, js_tag);
    void* mgr = duk_get_pointer(ctx, -1);
    duk_pop_2(ctx);
    return static_cast<AEManager*>(mgr);
}
std::string AEManager::defaultHandle(AEManager *mgr, const char *text) {
    LOG("[%p][%s]", mgr, text);
    return "";
}

AEManager::~AEManager() {
    duk_destroy_heap(_ctx);
}

AEManager::AEManager(GLuint width, GLuint height, GLfloat scale, const char* agent) {
    duk_context* ctx = duk_create_heap_default();
    duk_push_global_object(ctx);

    js_set_pptr(js_tag, this);
    js_set_strs("__agent",  agent);
    js_set_numb("__scale",  scale);
    js_set_uint("__width",  width);
    js_set_uint("__height", height);
    
    js_set_func(0, "__now",     now);       // GlobFunction
    js_set_func(1, "__alert",   alert);
    js_set_func(3, "__speak",   speak);
    js_set_func(1, "__handle",  handle);
    js_set_func(1, "__execute", execute);
    js_set_func(0, "__garbage", garbage);
    js_set_func(2, "__println", println);
    js_set_func(1, "__delItem", delItem);   // localStorage
    js_set_func(1, "__getItem", getItem);
    js_set_func(2, "__setItem", setItem);

    js_set_func(1, "escape",	escape);
    js_set_func(1, "unescape",	unescape);
    js_set_func(1, "decodeURI",	decodeURI);
    js_set_func(1, "encodeURI",	encodeURI);
    js_set_func(1, "decodeURIComponent", decodeURIComponent);
    js_set_func(1, "encodeURIComponent", encodeURIComponent);
    
    // --------------- eule_xxxx(...) -----------------
    js_set_func(3, "eule_setFromRotationMatrix",    eule_setFromRotationMatrix);
    js_set_func(3, "eule_setFromQuaternion",        eule_setFromQuaternion);
    js_set_func(3, "eule_reorder",                  eule_reorder);
    
    // --------------- mat3_xxxx(...) -----------------
    js_set_func(1, "mat3_identity",             mat3_identity);
    js_set_func(2, "mat3_setFromMatrix4",       mat3_setFromMatrix4);
    js_set_func(4, "mat3_applyToVector3Array",  mat3_applyToVector3Array);
    js_set_func(2, "mat3_multiplyScalar",       mat3_multiplyScalar);
    js_set_func(1, "mat3_determinant",          mat3_determinant);
    js_set_func(2, "mat3_getInverse",           mat3_getInverse);
    js_set_func(1, "mat3_transpose",            mat3_transpose);
    js_set_func(2, "mat3_getNormalMatrix",      mat3_getNormalMatrix);
    js_set_func(2, "mat3_transposeIntoArray",   mat3_transposeIntoArray);
    js_set_func(3, "mat3_fromArray",            mat3_fromArray);
    js_set_func(3, "mat3_toArray",              mat3_toArray);
    
    // --------------- mat4_xxxx(...) -----------------
    js_set_func(1, "mat4_identity",                     mat4_identity);
    js_set_func(2, "mat4_extractRotation",              mat4_extractRotation);
    js_set_func(3, "mat4_makeRotationFromEuler",        mat4_makeRotationFromEuler);
    js_set_func(2, "mat4_makeRotationFromQuaternion",   mat4_makeRotationFromQuaternion);
    js_set_func(4, "mat4_lookAt",                       mat4_lookAt);
    js_set_func(2, "mat4_multiplyScalar",               mat4_multiplyScalar);
    js_set_func(3, "mat4_multiplyMatrices",             mat4_multiplyMatrices);
    js_set_func(4, "mat4_applyToVector3Array",          mat4_applyToVector3Array);
    js_set_func(1, "mat4_determinant",                  mat4_determinant);
    js_set_func(1, "mat4_transpose",                    mat4_transpose);
    js_set_func(2, "mat4_setPosition",                  mat4_setPosition);
    js_set_func(2, "mat4_getInverse",                   mat4_getInverse);
    js_set_func(2, "mat4_scale",                        mat4_scale);
    js_set_func(1, "mat4_getMaxScaleOnAxis",            mat4_getMaxScaleOnAxis);
    js_set_func(4, "mat4_makeTranslation",              mat4_makeTranslation);
    js_set_func(2, "mat4_makeRotationX",                mat4_makeRotationX);
    js_set_func(2, "mat4_makeRotationY",                mat4_makeRotationY);
    js_set_func(2, "mat4_makeRotationZ",                mat4_makeRotationZ);
    js_set_func(3, "mat4_makeRotationAxis",             mat4_makeRotationAxis);
    js_set_func(4, "mat4_makeScale",                    mat4_makeScale);
    js_set_func(4, "mat4_makeShear",                    mat4_makeShear);
    js_set_func(4, "mat4_compose",                      mat4_compose);
    js_set_func(7, "mat4_makePerspective",              mat4_makePerspective);
    js_set_func(7, "mat4_makeOrthographic",             mat4_makeOrthographic);
    js_set_func(2, "mat4_equals",                       mat4_equals);
    js_set_func(3, "mat4_fromArray",                    mat4_fromArray);
    js_set_func(3, "mat4_toArray",                      mat4_toArray);
    
    // --------------- quat_xxxx(...) -----------------
    js_set_func(3, "quat_setFromEuler",             quat_setFromEuler);
    js_set_func(3, "quat_setFromAxisAngle",         quat_setFromAxisAngle);
    js_set_func(2, "quat_setFromRotationMatrix",    quat_setFromRotationMatrix);
    js_set_func(3, "quat_setFromUnitVectors",       quat_setFromUnitVectors);
    js_set_func(1, "quat_inverse",                  quat_inverse);
    js_set_func(1, "quat_conjugate",                quat_conjugate);
    js_set_func(2, "quat_dot",                      quat_dot);
    js_set_func(1, "quat_length",                   quat_length);
    js_set_func(1, "quat_lengthSq",                 quat_lengthSq);
    js_set_func(1, "quat_normalize",                quat_normalize);
    js_set_func(3, "quat_multiplyQuaternions",      quat_multiplyQuaternions);
    js_set_func(3, "quat_slerp",                    quat_slerp);
    js_set_func(2, "quat_equals",                   quat_equals);
    js_set_func(3, "quat_fromArray",                quat_fromArray);
    js_set_func(3, "quat_toArray",                  quat_toArray);
    
    // --------------- vec3_xxxx(...) -----------------
    js_set_func(2, "vec3_add",                      vec3_add);
    js_set_func(2, "vec3_addScalar",                vec3_addScalar);
    js_set_func(3, "vec3_addVectors",               vec3_addVectors);
    js_set_func(3, "vec3_addScaledVector",          vec3_addScaledVector);
    js_set_func(2, "vec3_sub",                      vec3_sub);
    js_set_func(2, "vec3_subScalar",                vec3_subScalar);
    js_set_func(3, "vec3_subVectors",               vec3_subVectors);
    js_set_func(2, "vec3_multiply",                 vec3_multiply);
    js_set_func(2, "vec3_multiplyScalar",           vec3_multiplyScalar);
    js_set_func(3, "vec3_multiplyVectors",          vec3_multiplyVectors);
    js_set_func(3, "vec3_applyEuler",               vec3_applyEuler);
    js_set_func(3, "vec3_applyAxisAngle",           vec3_applyAxisAngle);
    js_set_func(2, "vec3_applyMatrix3",             vec3_applyMatrix3);
    js_set_func(2, "vec3_applyMatrix4",             vec3_applyMatrix4);
    js_set_func(2, "vec3_applyProjection",          vec3_applyProjection);
    js_set_func(2, "vec3_applyQuaternion",          vec3_applyQuaternion);
    js_set_func(3, "vec3_project",                  vec3_project);
    js_set_func(3, "vec3_unproject",                vec3_unproject);
    js_set_func(2, "vec3_transformDirection",       vec3_transformDirection);
    js_set_func(2, "vec3_divide",                   vec3_divide);
    js_set_func(2, "vec3_divideScalar",             vec3_divideScalar);
    js_set_func(2, "vec3_min",                      vec3_min);
    js_set_func(2, "vec3_max",                      vec3_max);
    js_set_func(3, "vec3_clamp",                    vec3_clamp);
    js_set_func(3, "vec3_clampScalar",              vec3_clampScalar);
    js_set_func(3, "vec3_clampLength",              vec3_clampLength);
    js_set_func(1, "vec3_floor",                    vec3_floor);
    js_set_func(1, "vec3_ceil",                     vec3_ceil);
    js_set_func(1, "vec3_round",                    vec3_round);
    js_set_func(1, "vec3_roundToZero",              vec3_roundToZero);
    js_set_func(1, "vec3_negate",                   vec3_negate);
    js_set_func(2, "vec3_dot",                      vec3_dot);
    js_set_func(1, "vec3_length",                   vec3_length);
    js_set_func(1, "vec3_lengthSq",                 vec3_lengthSq);
    js_set_func(1, "vec3_lengthManhattan",          vec3_lengthManhattan);
    js_set_func(1, "vec3_normalize",                vec3_normalize);
    js_set_func(2, "vec3_setLength",                vec3_setLength);
    js_set_func(3, "vec3_lerp",                     vec3_lerp);
    js_set_func(4, "vec3_lerpVectors",              vec3_lerpVectors);
    js_set_func(2, "vec3_cross",                    vec3_cross);
    js_set_func(3, "vec3_crossVectors",             vec3_crossVectors);
    js_set_func(2, "vec3_projectOnVector",          vec3_projectOnVector);
    js_set_func(2, "vec3_projectOnPlane",           vec3_projectOnPlane);
    js_set_func(2, "vec3_reflect",                  vec3_reflect);
    js_set_func(2, "vec3_angleTo",                  vec3_angleTo);
    js_set_func(2, "vec3_distanceTo",               vec3_distanceTo);
    js_set_func(2, "vec3_distanceToSquared",        vec3_distanceToSquared);
    js_set_func(2, "vec3_distanceToManhattan",      vec3_distanceToManhattan);
    js_set_func(2, "vec3_setFromMatrixPosition",    vec3_setFromMatrixPosition);
    js_set_func(2, "vec3_setFromMatrixScale",       vec3_setFromMatrixScale);
    js_set_func(3, "vec3_setFromMatrixColumn",      vec3_setFromMatrixColumn);
    js_set_func(2, "vec3_equals",                   vec3_equals);
    js_set_func(3, "vec3_fromArray",                vec3_fromArray);
    js_set_func(3, "vec3_toArray",                  vec3_toArray);
    
    // --------------- vec4_xxxx(...) -----------------
    js_set_func(2, "vec4_add",                              vec4_add);
    js_set_func(2, "vec4_addScalar",                        vec4_addScalar);
    js_set_func(3, "vec4_addVectors",                       vec4_addVectors);
    js_set_func(3, "vec4_addScaledVector",                  vec4_addScaledVector);
    js_set_func(2, "vec4_sub",                              vec4_sub);
    js_set_func(2, "vec4_subScalar",                        vec4_subScalar);
    js_set_func(3, "vec4_subVectors",                       vec4_subVectors);
    js_set_func(2, "vec4_multiplyScalar",                   vec4_multiplyScalar);
    js_set_func(2, "vec4_applyMatrix4",                     vec4_applyMatrix4);
    js_set_func(2, "vec4_divideScalar",                     vec4_divideScalar);
    js_set_func(2, "vec4_setAxisAngleFromQuaternion",       vec4_setAxisAngleFromQuaternion);
    js_set_func(2, "vec4_setAxisAngleFromRotationMatrix",   vec4_setAxisAngleFromRotationMatrix);
    js_set_func(2, "vec4_min",                              vec4_min);
    js_set_func(2, "vec4_max",                              vec4_max);
    js_set_func(3, "vec4_clamp",                            vec4_clamp);
    js_set_func(3, "vec4_clampScalar",                      vec4_clampScalar);
    js_set_func(1, "vec4_floor",                            vec4_floor);
    js_set_func(1, "vec4_ceil",                             vec4_ceil);
    js_set_func(1, "vec4_round",                            vec4_round);
    js_set_func(1, "vec4_roundToZero",                      vec4_roundToZero);
    js_set_func(1, "vec4_negate",                           vec4_negate);
    js_set_func(2, "vec4_dot",                              vec4_dot);
    js_set_func(1, "vec4_length",                           vec4_length);
    js_set_func(1, "vec4_lengthSq",                         vec4_lengthSq);
    js_set_func(1, "vec4_lengthManhattan",                  vec4_lengthManhattan);
    js_set_func(1, "vec4_normalize",                        vec4_normalize);
    js_set_func(2, "vec4_setLength",                        vec4_setLength);
    js_set_func(3, "vec4_lerp",                             vec4_lerp);
    js_set_func(4, "vec4_lerpVectors",                      vec4_lerpVectors);
    js_set_func(2, "vec4_equals",                           vec4_equals);
    js_set_func(3, "vec4_fromArray",                        vec4_fromArray);
    js_set_func(3, "vec4_toArray",                          vec4_toArray);
    
    duk_pop(ctx);
    
    AESound::binding(ctx);
    AEImageData::binding(ctx);
    AEVRDisplay::binding(ctx);
    AEWebSocket::binding(ctx);
    AEXMLHttpRequest::binding(ctx);
    AESpeechSynthesis::binding(ctx);
    AEHTMLAudioElement::binding(ctx);
    AEHTMLImageElement::binding(ctx);
    AEHTMLVideoElement::binding(ctx);
    AEWebGLRenderingContext::binding(ctx);
    AECanvasRenderingContext2D::binding(ctx);
    
    _ctx    = ctx;
    _handle = defaultHandle;
}

void AEManager::init() {
    execute("file:///asset/AppEngine.bundle/window.js");
}

void AEManager::jsgc() {
    LOG("js gc");
    duk_gc(_ctx, 0);
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
void AEManager::check() {
    for (GLint error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
        LOG("glError: 0x%x.", error);
    }
}
void AEManager::setHandle(AEManagerHandle handle) {
    _handle = handle;
}
bool AEManager::execute(const char* url_) {
    std::string url = absURL(url_);
    char* txt = AEPlatform::textWithURL(this, url.c_str());
    bool  err = duk_peval_string(_ctx, txt);
    LOG("[%s] %s.", url.c_str(), err ? duk_safe_to_string(_ctx, -1) : "success");
    duk_pop(_ctx);
    free(txt);
    return !err;
}
void AEManager::loadURL(const char *url) {
    _url.reset(url);
    duk_push_global_object(_ctx);
    duk_push_string(_ctx, "__loadURL");
    duk_push_string(_ctx, url);
    duk_push_string(_ctx, _url.scheme().c_str());
    duk_push_string(_ctx, _url.host().c_str());
    duk_push_string(_ctx, _url.port().c_str());
    duk_push_string(_ctx, _url.path().c_str());
    duk_push_string(_ctx, _url.query().c_str());
    duk_push_string(_ctx, _url.fragment().c_str());
    if (duk_pcall_prop(_ctx, -9, 7)) {
        LOG("[%s][%s]", url, duk_safe_to_string(_ctx, -1));
    }
    duk_pop_2(_ctx);
}
void AEManager::applyTimer() {
    duk_push_global_object(_ctx);
    duk_push_string(_ctx, "RAFExec");
    if (duk_pcall_prop(_ctx, -2, 0)) {
    	LOG("%s", duk_safe_to_string(_ctx, -1));
    }
    duk_pop(_ctx);

    duk_push_string(_ctx, "__applyTimer");
    if (duk_pcall_prop(_ctx, -2, 0)) {
        LOG("%s", duk_safe_to_string(_ctx, -1));
    }
    duk_pop_2(_ctx);
}
void AEManager::applyResize(GLuint width, GLuint height) {
    duk_push_global_object(_ctx);
    duk_push_string(_ctx, "__applyResize");
    duk_push_uint(_ctx, width);
    duk_push_uint(_ctx, height);
    if (duk_pcall_prop(_ctx, -4, 2)) {
        LOG("%s", duk_safe_to_string(_ctx, -1));
    }
    duk_pop_2(_ctx);
}
void AEManager::applyKeyboard(AEEvent::Type type, GLint keyCode) {
    duk_push_global_object(_ctx);
    duk_push_string(_ctx, "__applyKeyboard");
    duk_push_string(_ctx, event(type));
    duk_push_int(_ctx, keyCode);
    if (duk_pcall_prop(_ctx, -4, 2)) {
        LOG("%s", duk_safe_to_string(_ctx, -1));
    }
    duk_pop_2(_ctx);
}

template<typename Handle>
void AEManager::merge(AEEvent::Type type, GLint num, AETouch* pts, Handle handle) {
    if (type == AEEvent::MouseUp || type == AEEvent::MouseOut || type == AEEvent::TouchEnd || type == AEEvent::TouchCancel) {
        for (int idx = 0; idx < num; idx++) {
            auto it = _touches.find(pts[idx].ID);
            if (it != _touches.end()) {
                _touches.erase(it);
            }
        }
        handle(type, num, pts);
    }
    else {
        for (int idx = 0; idx < num; idx++) {
            _touches[pts[idx].ID] = pts[idx];
        }
        GLint idx = 0;
        GLint num = static_cast<GLint>(_touches.size());
        AETouch* touches = (AETouch*)malloc(num * sizeof(AETouch));
        for (auto it = _touches.begin(); it != _touches.end();) {
            touches[idx] = it->second;
            it++;
            idx++;
        }
        handle(type, num, touches);
        free(touches);
    }
}
void AEManager::touch(AEEvent::Type type, GLint len, AETouch* pts) {
    merge(type, len, pts, [&](AEEvent::Type type, GLint num, AETouch* touches) {
        duk_push_global_object(_ctx);
        duk_push_string(_ctx, "__touch");
        duk_push_string(_ctx, event(type));
        duk_idx_t array;
        array = duk_push_array(_ctx);
        for (GLuint idx = 0; idx < num; idx++) {
            duk_push_number(_ctx, touches[idx].x);
            duk_put_prop_index(_ctx, array, idx);
        }
        array = duk_push_array(_ctx);
        for (GLuint idx = 0; idx < num; idx++) {
            duk_push_number(_ctx, touches[idx].y);
            duk_put_prop_index(_ctx, array, idx);
        }
        if (duk_pcall_prop(_ctx, -5, 3)) {
            LOG("%s", duk_safe_to_string(_ctx, -1));
        }
        duk_pop_2(_ctx);
    });
}
void AEManager::orientation(double alpha, double beta, double gamma) {
    duk_push_global_object(_ctx);
    duk_push_string(_ctx, "__orientation");
    duk_push_number(_ctx, alpha);
    duk_push_number(_ctx, beta);
    duk_push_number(_ctx, gamma);
    if (duk_pcall_prop(_ctx, -5, 3)) {
        LOG("%s", duk_safe_to_string(_ctx, -1));
    }
    duk_pop_2(_ctx);
}

std::string AEManager::absURL(const char* name) {
    return _url.absoluteURL(name ? name : "");
}
std::string AEManager::evalute(const char* text) {
    const bool  err = duk_peval_string(_ctx, text);
    std::string msg = duk_safe_to_string(_ctx, -1);
    if (err) {
        LOG("[%s][%s]", text, msg.c_str());
    }
    duk_pop(_ctx);
    return msg;
}

int AEManager::now(duk_context* ctx) {
    duk_push_number(ctx, get_now());
    return 1;
}
int AEManager::alert(duk_context* ctx) {
    const char* text = duk_get_string(ctx, 0);
    AEPlatform::alert(find(ctx), text);
    return 0;
}
int AEManager::speak(duk_context* ctx) {
    const char* text = duk_get_string(ctx, 0);
    const char* lang = duk_get_string(ctx, 1);
    const float rate = duk_get_number(ctx, 2);
    AEPlatform::speak(find(ctx), text, lang, rate);
    return 0;
}
int AEManager::handle(duk_context* ctx) {
    AEManager*  mgr = AEManager::find(ctx);
    const char* txt = duk_get_string(ctx, 0);
    if (mgr && mgr->_handle) {
        std::string ret = mgr->_handle(mgr, txt);
        duk_push_string(ctx, ret.c_str());
    }
    else {
        duk_push_string(ctx, "");
    }
    return 1;
}
int AEManager::execute(duk_context* ctx) {
    const char* url = duk_get_string(ctx, 0);
    AEManager::find(ctx)->execute(url);
    return 0;
}
int AEManager::garbage(duk_context* ctx) {
    duk_gc(ctx, 0);
    return 0;
}
int AEManager::println(duk_context* ctx) {
    const GLint type = duk_get_int(ctx, 0);
    const char* text = duk_get_string(ctx, 1);
    switch (type) {
        case 0: {
            LOGD("%s", text);
            break;
        }
        case 1: {
            LOGI("%s", text);
            break;
        }
        case 2: {
            LOGW("%s", text);
            break;
        }
        case 3: {
            LOGE("%s", text);
            break;
        }
        default: {
            LOG("%s", text);
            break;
        }
    }
    return 0;
}
int AEManager::delItem(duk_context* ctx) {
    const char* name = duk_get_string(ctx, 0);
    AEPlatform::delItem(find(ctx), name);
    return true;
}
int AEManager::getItem(duk_context* ctx) {
    const char* name = duk_get_string(ctx, 0);
    duk_push_string(ctx, AEPlatform::getItem(find(ctx), name) ?: "");
    return 1;
}
int AEManager::setItem(duk_context* ctx) {
    const char* name = duk_get_string(ctx, 0);
    const char* text = duk_get_string(ctx, 1);
    AEPlatform::setItem(find(ctx), name, text);
    return 0;
}

int AEManager::escape(duk_context* ctx) {
	return encodeURI(ctx);
}
int AEManager::unescape(duk_context* ctx) {
	return decodeURI(ctx);
}
int AEManager::decodeURI(duk_context* ctx) {
	const char* str = duk_get_string(ctx, 0);
	std::string val = AEPlatform::decodeURL(str);
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEManager::encodeURI(duk_context* ctx) {
	const char* str = duk_get_string(ctx, 0);
	std::string val = AEPlatform::encodeURL(str);
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEManager::decodeURIComponent(duk_context* ctx) {
	return decodeURI(ctx);
}
int AEManager::encodeURIComponent(duk_context* ctx) {
	return encodeURI(ctx);
}
