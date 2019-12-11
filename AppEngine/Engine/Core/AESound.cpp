//
//  AESound.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AESound.hpp"
#include "AEManager.hpp"

AESound::~AESound() {
    delete bean;
}
AESound::AESound() {
	cx   = nullptr;
    bean = new AESoundBean(this);
}
void AESound::init(duk_context* ctx) {
    bean->init(AEManager::find(ctx));
}
int AESound::get_muted(duk_context* ctx) {
    js_thiz(AESound);
    bool val = thiz->bean->getMuted();
    duk_push_boolean(ctx, val);
    return 1;
}
int AESound::set_muted(duk_context* ctx) {
    js_thiz(AESound);
    bool val = duk_get_boolean(ctx, 0);
    thiz->bean->setMuted(val);
    return 0;
}
int AESound::get_volume(duk_context* ctx) {
    js_thiz(AESound);
    float val = thiz->bean->getVolume();
    duk_push_number(ctx, val);
    return 1;
}
int AESound::set_volume(duk_context* ctx) {
    js_thiz(AESound);
    float val = duk_get_number(ctx, 0);
    thiz->bean->setVolume(val);
    return 0;
}
int AESound::load(duk_context* ctx) {
    js_thiz(AESound);
    const char* val = duk_get_string(ctx, 0);
    thiz->bean->preload(val);
    return 0;
}
int AESound::play(duk_context* ctx) {
    js_thiz(AESound);
    const char* url = duk_get_string(ctx, 0);
    const uint  val = thiz->bean->play(url);
    duk_push_uint(ctx, val);
    return 1;
}
int AESound::stop(duk_context* ctx) {
    js_thiz(AESound);
//    uint val = duk_get_uint(ctx, 0);
    thiz->bean->stop(nullptr);
    return 0;
}
void AESound::binding(duk_context* ctx) {
    js_begin_binding();
    js_add_prop(muted);
    js_add_prop(volume);
    js_binding_func(1, load);
    js_binding_func(1, play);
    js_binding_func(1, stop);
    js_end_binding(AESound);
}
