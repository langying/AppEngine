//
//  AEHTMLVideoElement.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/8/23.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEManager.hpp"
#include "AEHTMLVideoElement.hpp"

AEHTMLVideoElement::~AEHTMLVideoElement() {
	bean->release();
	delete bean;
}
AEHTMLVideoElement::AEHTMLVideoElement() : cx(nullptr) {
	ID = AEPlatform::UUID();
	width = height = duration = 0;
	readyState = 1;
	defaultPlaybackRate = 1;
	bean = new AEHTMLVideoElementBean(this);
}
void AEHTMLVideoElement::init(duk_context* ctx) {
	bean->init(AEManager::find(ctx));
}
void AEHTMLVideoElement::onFinish(GLuint width, GLuint height, GLuint duration) {
	this->width      = width;
	this->height     = height;
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
void AEHTMLVideoElement::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
	GLint texId = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &texId);
	bean->texImage2D(texId, target, level, infmt, format, type);
}
void AEHTMLVideoElement::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
	bean->texSubImage2D(target, level, xoffset, yoffset, format, type);
}

int AEHTMLVideoElement::get_src(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	std::string val = thiz->bean->getSrc();
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEHTMLVideoElement::set_src(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	const char* val = duk_get_string(ctx, 0);
	thiz->bean->setSrc(val);

    duk_push_global_stash(ctx);             // ... stash
    duk_push_this(ctx);                     // ... stash this
    duk_put_prop_index(ctx, -2, thiz->ID);  // ... stash ([ID]=this)
    duk_pop(ctx);                           // ...
	return 0;
}
int AEHTMLVideoElement::get_loop(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = thiz->bean->getLoop();
	duk_push_boolean(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_loop(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = duk_get_boolean(ctx, 0);
	thiz->bean->setLoop(val);
	return 0;
}
int AEHTMLVideoElement::get_muted(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = thiz->bean->getMuted();
	duk_push_boolean(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_muted(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = duk_get_boolean(ctx, 0);
	thiz->bean->setMuted(val);
	return 0;
}
int AEHTMLVideoElement::get_paused(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = thiz->bean->getPaused();
	duk_push_boolean(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_paused(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = duk_get_boolean(ctx, 0);
	thiz->bean->setPaused(val);
	return 0;
}
int AEHTMLVideoElement::get_volume(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	float val = thiz->bean->getVolume();
	duk_push_number(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_volume(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	float val = duk_get_number(ctx, 0);
	thiz->bean->setVolume(val);
	return 0;
}
int AEHTMLVideoElement::get_autoplay(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = thiz->bean->getAutoplay();
	duk_push_boolean(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_autoplay(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	bool val = duk_get_boolean(ctx, 0);
	thiz->bean->setAutoplay(val);
	return 0;
}
int AEHTMLVideoElement::get_currentTime(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	uint val = thiz->bean->getCurrentTime();
	duk_push_uint(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_currentTime(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	uint val = duk_get_uint(ctx, 0);
	thiz->bean->setCurrentTime(val);
	return 0;
}
int AEHTMLVideoElement::get_playbackRate(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	float val = thiz->bean->getPlaybackRate();
	duk_push_number(ctx, val);
	return 1;
}
int AEHTMLVideoElement::set_playbackRate(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	float val = duk_get_number(ctx, 0);
	thiz->bean->setPlaybackRate(val);
	return 0;
}

int AEHTMLVideoElement::load(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	thiz->bean->load();
	return 0;
}
int AEHTMLVideoElement::play(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	thiz->bean->play();
	return 0;
}
int AEHTMLVideoElement::stop(duk_context* ctx) {
	js_thiz(AEHTMLVideoElement);
	thiz->bean->stop();
	return 0;
}

void AEHTMLVideoElement::binding(duk_context *ctx) {
	js_begin_binding();
	js_add_prop(width);
	js_add_prop(height);
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

	js_add_alias(width,  videoWidth);
	js_add_alias(height, videoHeight);
	js_add_value(int, 0, HAVE_NOTHING);
	js_add_value(int, 1, HAVE_METADATA);
	js_add_value(int, 2, HAVE_CURRENT_DATA);
	js_add_value(int, 3, HAVE_FUTURE_DATA);
	js_add_value(int, 4, HAVE_ENOUGH_DATA);

	js_binding_func(0, load);
	js_binding_func(0, play);
	js_binding_func(0, stop);
	js_end_binding(HTMLVideoElement);
}
