//
//  AESpeechSynthesis.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/11.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEManager.hpp"
#include "AESpeechSynthesis.hpp"

AESpeechSynthesis::~AESpeechSynthesis() {
	bean->release();
	delete bean;
}
AESpeechSynthesis::AESpeechSynthesis() : cx(nullptr) {
	bean = new AESpeechSynthesisBean(this);
}
void AESpeechSynthesis::init(duk_context* ctx) {
	bean->init(AEManager::find(ctx));
}

int AESpeechSynthesis::pause(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	thiz->bean->pause();
	return 0;
}
int AESpeechSynthesis::speak(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	duk_get_prop_string(ctx, 0, "rate");
	const float rate = duk_get_number(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, 0, "lang");
	const char* lang = duk_get_string(ctx, -1);
	duk_pop(ctx);
	duk_get_prop_string(ctx, 0, "text");
	const char* text = duk_get_string(ctx, -1);
	duk_pop(ctx);

	thiz->bean->speak(rate, lang, text);
	return 0;
}
int AESpeechSynthesis::cancel(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	thiz->bean->cancel();
	return 0;
}
int AESpeechSynthesis::resume(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	thiz->bean->resume();
	return 0;
}
int AESpeechSynthesis::getVoices(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	thiz->bean->getVoices();
	duk_push_array(ctx);
	return 0;
}

int AESpeechSynthesis::get_paused(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	bool val = thiz->bean->paused();
	duk_push_boolean(ctx, val);
	return 1;
}
int AESpeechSynthesis::set_paused(duk_context* ctx) {
	return 0;
}
int AESpeechSynthesis::get_pending(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	bool val = thiz->bean->pending();
	duk_push_boolean(ctx, val);
	return 1;
}
int AESpeechSynthesis::set_pending(duk_context* ctx) {
	return 0;
}
int AESpeechSynthesis::get_speaking(duk_context* ctx) {
	js_thiz(AESpeechSynthesis);
	bool val = thiz->bean->speaking();
	duk_push_boolean(ctx, val);
	return 1;
}
int AESpeechSynthesis::set_speaking(duk_context* ctx) {
	return 0;
}

void AESpeechSynthesis::binding(duk_context *ctx) {
	js_begin_binding();
	js_add_prop(paused);
	js_add_prop(pending);
	js_add_prop(speaking);

	js_binding_func(0, pause);
	js_binding_func(1, speak);
	js_binding_func(0, cancel);
	js_binding_func(0, resume);
	js_binding_func(0, getVoices);

	js_end_binding(SpeechSynthesis);
}
