//
//  AEImageData.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEFormat.hpp"
#include "AEImageData.hpp"

AEImageData::~AEImageData() {
	ae_free(buffer);
}
AEImageData::AEImageData(): cx(nullptr) {
	width  = 0;
	height = 0;
	buffer = nullptr;
}
int AEImageData::get_data(duk_context* ctx) {
	js_thiz(AEImageData);
	uint len = thiz->width * thiz->height * 4;
    duk_push_external_buffer(ctx);									// stack [... buffer]
    duk_config_buffer(ctx, -1, thiz->buffer, len);					// stack [... buffer]
    duk_push_buffer_object(ctx, -1, 0, len, DUK_BUFOBJ_UINT8ARRAY);	// stack [... buffer array]
    duk_remove(ctx, -2);											// stack [... array ]
    return 1;
}
int AEImageData::set_data(duk_context* ctx) {
	return 0;
}
void AEImageData::resize(GLuint width, GLuint height) {
	ae_free(buffer);
	this->width  = width;
	this->height = height;
	this->buffer = calloc(width * height, sizeof(GLuint));
}
void AEImageData::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
    AEFormat::convert(buffer, width, height, flipY, alpha);
	glTexImage2D(target, level, infmt, width, height, 0, format, type, buffer);
}
void AEImageData::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
    AEFormat::convert(buffer, width, height, flipY, alpha);
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, buffer);
}

void AEImageData::binding(duk_context* ctx) {
    js_begin_binding();
    js_add_prop(data);
    js_add_prop(width);
    js_add_prop(height);
    js_end_binding(ImageData);
}
