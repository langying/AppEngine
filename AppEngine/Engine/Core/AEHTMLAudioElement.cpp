//
//  AEHTMLAudioElement.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEManager.hpp"
#include "AEHTMLAudioElement.hpp"

AEHTMLAudioElement::~AEHTMLAudioElement() {
    bean->release();
    delete bean;
}
AEHTMLAudioElement::AEHTMLAudioElement() : cx(nullptr) {
    ID = AEPlatform::UUID();
    duration = 0;
    readyState = 1;
    defaultPlaybackRate = 1;
    bean = new AEHTMLAudioElementBean(this);
}
void AEHTMLAudioElement::init(duk_context* ctx) {
    bean->init(AEManager::find(ctx));
}
void AEHTMLAudioElement::onFinish(GLuint width, GLuint height, GLuint duration) {
    this->duration   = duration;
    this->readyState = 4;
    
    duk_push_global_stash(cx);      // ... stash
    duk_get_prop_index(cx, -1, ID); // ... stash this
    duk_push_string(cx, "onFinish");// ... stash this onFinish
    duk_call_prop(cx, -2, 0);       // ... stash this result
    duk_pop_2(cx);                  // ... stash
    duk_del_prop_index(cx, -1, ID); // ... stash (delete stash[ID])
    duk_pop(cx);					// ...
}

int AEHTMLAudioElement::get_src(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    std::string val = thiz->bean->getSrc();
    duk_push_string(ctx, val.c_str());
    return 1;
}
int AEHTMLAudioElement::set_src(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    const char* val = duk_get_string(ctx, 0);
    thiz->bean->setSrc(val);
    
    duk_push_global_stash(ctx);             // ... stash
    duk_push_this(ctx);                     // ... stash this
    duk_put_prop_index(ctx, -2, thiz->ID);  // ... stash ([ID]=this)
    duk_pop(ctx);                           // ...
    return 0;
}
int AEHTMLAudioElement::get_loop(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = thiz->bean->getLoop();
    duk_push_boolean(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_loop(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = duk_get_boolean(ctx, 0);
    thiz->bean->setLoop(val);
    return 0;
}
int AEHTMLAudioElement::get_muted(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = thiz->bean->getMuted();
    duk_push_boolean(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_muted(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = duk_get_boolean(ctx, 0);
    thiz->bean->setMuted(val);
    return 0;
}
int AEHTMLAudioElement::get_paused(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = thiz->bean->getPaused();
    duk_push_boolean(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_paused(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = duk_get_boolean(ctx, 0);
    thiz->bean->setPaused(val);
    return 0;
}
int AEHTMLAudioElement::get_volume(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    float val = thiz->bean->getVolume();
    duk_push_number(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_volume(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    float val = duk_get_number(ctx, 0);
    thiz->bean->setVolume(val);
    return 0;
}
int AEHTMLAudioElement::get_autoplay(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = thiz->bean->getAutoplay();
    duk_push_boolean(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_autoplay(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    bool val = duk_get_boolean(ctx, 0);
    thiz->bean->setAutoplay(val);
    return 0;
}
int AEHTMLAudioElement::get_currentTime(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    uint val = thiz->bean->getCurrentTime();
    duk_push_uint(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_currentTime(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    uint val = duk_get_uint(ctx, 0);
    thiz->bean->setCurrentTime(val);
    return 0;
}
int AEHTMLAudioElement::get_playbackRate(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    float val = thiz->bean->getPlaybackRate();
    duk_push_number(ctx, val);
    return 1;
}
int AEHTMLAudioElement::set_playbackRate(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    float val = duk_get_number(ctx, 0);
    thiz->bean->setPlaybackRate(val);
    return 0;
}

int AEHTMLAudioElement::load(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    thiz->bean->load();
    return 0;
}
int AEHTMLAudioElement::play(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    thiz->bean->play();
    return 0;
}
int AEHTMLAudioElement::stop(duk_context* ctx) {
    js_thiz(AEHTMLAudioElement);
    thiz->bean->stop();
    return 0;
}

void AEHTMLAudioElement::binding(duk_context *ctx) {
    js_begin_binding();
    js_add_prop(duration);
    js_add_prop(readyState);
    js_add_prop(defaultPlaybackRate);
    
    js_add_prop(src);
    js_add_prop(loop);
    js_add_prop(muted);
    js_add_prop(paused);
    js_add_prop(volume);
    js_add_prop(autoplay);
    js_add_prop(currentTime);
    js_add_prop(playbackRate);
    
    js_add_value(int, 0, HAVE_NOTHING);
    js_add_value(int, 1, HAVE_METADATA);
    js_add_value(int, 2, HAVE_CURRENT_DATA);
    js_add_value(int, 3, HAVE_FUTURE_DATA);
    js_add_value(int, 4, HAVE_ENOUGH_DATA);
    
    js_binding_func(0, load);
    js_binding_func(0, play);
    js_binding_func(0, stop);
    js_end_binding(HTMLAudioElement);
}

