//
//  AEImage.cpp
//  Action
//
//  Created by 韩琼 on 16/1/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEManager.hpp"
#include "AEHTMLImageElement.hpp"

AEHTMLImageElement::~AEHTMLImageElement() {
}
AEHTMLImageElement::AEHTMLImageElement(): AEImage() {
	cx    = nullptr;
    ID    = AEPlatform::UUID();
    bean  = new AEHTMLImageElementBean(this);

    width    = 0;
    height   = 0;
    complete = false;
}
void AEHTMLImageElement::init(AEManager* mgr) {
	bean->init(mgr);
}
void AEHTMLImageElement::onFinish(GLuint width, GLuint height) {
	this->width    = width;
	this->height   = height;
	this->complete = true;

    duk_push_global_stash(cx);      // ... stash
    duk_get_prop_index(cx, -1, ID); // ... stash this
    duk_push_string(cx, "onFinish");// ... stash this onFinish
    duk_call_prop(cx, -2, 0);       // ... stash this result
    duk_pop_2(cx);                  // ... stash
    duk_del_prop_index(cx, -1, ID); // ... stash (delete stash[ID])
    duk_pop(cx);                    // ...
}
void AEHTMLImageElement::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha) {
	bean->texImage2D(target, level, infmt, format, type, unpackFlipY, unpackPremultiplyAlpha);
}
void AEHTMLImageElement::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha) {
	bean->texSubImage2D(target, level, xoffset, yoffset, format, type, unpackFlipY, unpackPremultiplyAlpha);
}
std::string AEHTMLImageElement::pathfile() {
    return bean->src;
}

int AEHTMLImageElement::get_src(duk_context* ctx) {
    js_thiz(AEHTMLImageElement);
    const char* val = thiz->bean->src.c_str();
    duk_push_string(ctx, val);
    return 1;
}
int AEHTMLImageElement::set_src(duk_context* ctx) {
    js_thiz(AEHTMLImageElement);			// ...
    duk_push_global_stash(ctx);             // ... stash
    duk_push_this(ctx);                     // ... stash this
    duk_put_prop_index(ctx, -2, thiz->ID);  // ... stash ([ID]=this)
    duk_pop(ctx);                           // ...

    const char* src = duk_get_string(ctx, 0);
    thiz->bean->setSrc(src);
    return 0;
}

void AEHTMLImageElement::binding(duk_context* ctx) {
    js_begin_binding();
    js_add_prop(src);
    js_add_prop(width);
    js_add_prop(height);
    js_add_prop(complete);
    js_end_binding(HTMLImageElement);
}
