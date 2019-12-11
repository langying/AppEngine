//
//  AECanvasRenderingContext2D.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/1/12.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEFormat.hpp"
#include "AEImageData.hpp"
#include "AECanvasRenderingContext2D.hpp"

#define duk_get_str(ctx, idx) duk_get_string(ctx, idx)

const char p_data[]		= "data";
const char p_width[]	= "width";
const char p_height[]	= "height";

AECanvasRenderingContext2D::~AECanvasRenderingContext2D() {
}
AECanvasRenderingContext2D::AECanvasRenderingContext2D(): AEImage() {
	cx = nullptr;
	bean = new AECanvasRenderingContext2DBean();
	width = height = 0;
}
void AECanvasRenderingContext2D::init(AEManager* mgr) {
	bean->init(mgr);
}
void AECanvasRenderingContext2D::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
	bean->texImage2D(target, level, infmt, format, type, flipY, alpha);
}
void AECanvasRenderingContext2D::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
	bean->texSubImage2D(target, level, xoffset, yoffset, format, type, flipY, alpha);
}

int AECanvasRenderingContext2D::get_font(duk_context *ctx) {
    js_thiz(AECanvasRenderingContext2D);
    std::string style = "";
    style += thiz->bean->getFontSize();
    style += "px ";
    style += thiz->bean->getFontName();
    duk_push_string(ctx, style.c_str());
    return 1;
}
int AECanvasRenderingContext2D::set_font(duk_context *ctx) {
    js_thiz(AECanvasRenderingContext2D);
    const char* style = duk_get_string(ctx, 0);
    AEFormat::font(style, [thiz](GLfloat size, const char* name) {
        if (size) {
            thiz->bean->setFontSize(size);
        }
        if (name) {
            thiz->bean->setFontName(name);
        }
    });
    return 0;
}
int AECanvasRenderingContext2D::get_fillStyle(duk_context *ctx) {
	js_thiz(AECanvasRenderingContext2D);
	std::string val = thiz->bean->getFillStyle();
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AECanvasRenderingContext2D::set_fillStyle(duk_context *ctx) {
	js_thiz(AECanvasRenderingContext2D);
	const char* style = duk_get_string(ctx, 0);
	const uint  color = static_cast<uint>(strtol(style, NULL, 16));
	thiz->bean->setFillColor(color);
	thiz->bean->setFillStyle(style);
	return 0;
}
int AECanvasRenderingContext2D::get_lineWidth(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	float val = thiz->bean->getLineWidth();
	duk_push_number(ctx, val);
	return 1;
}
int AECanvasRenderingContext2D::set_lineWidth(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	float val = duk_get_number(ctx, 0);
	thiz->bean->setLineWidth(val);
	return 0;
}
int AECanvasRenderingContext2D::get_textBaseline(duk_context *ctx) {
	LOGW("not support %s.", "alphabetic");
	duk_push_string(ctx, "alphabetic");
	return 1;
}
int AECanvasRenderingContext2D::set_textBaseline(duk_context *ctx) {
	LOGW("not support %s.", duk_get_string(ctx, 0));
	return 0;
}

int AECanvasRenderingContext2D::size(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	thiz->width  = duk_get_uint(ctx, 0);
	thiz->height = duk_get_uint(ctx, 1);
    thiz->bean->resize(thiz->width, thiz->height);
	return 0;
}
int AECanvasRenderingContext2D::clearRect(duk_context *ctx) {
	js_thiz(AECanvasRenderingContext2D);
	GLint x = duk_get_int(ctx, 0);
	GLint y = duk_get_int(ctx, 1);
	GLint w = duk_get_int(ctx, 2);
	GLint h = duk_get_int(ctx, 3);
    thiz->bean->clearRect(x, y, w, h);
	return 0;
}
int AECanvasRenderingContext2D::drawImage(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	js_that(0, AEImage, img);
	GLint argc = duk_get_top(ctx);
	GLint sx, sy, sw, sh, dx, dy, dw, dh;
	if (argc == 9) {
		sx = duk_get_int(ctx, 1);
		sy = duk_get_int(ctx, 2);
		sw = duk_get_int(ctx, 3);
		sh = duk_get_int(ctx, 4);
		dx = duk_get_int(ctx, 5);
		dy = duk_get_int(ctx, 6);
		dw = duk_get_int(ctx, 7);
		dh = duk_get_int(ctx, 8);
	}
	else if (argc == 5) {
		sx = 0;
		sy = 0;
		sw = img->get_width();
		sh = img->get_height();
		dx = duk_get_int(ctx, 1);
		dy = duk_get_int(ctx, 2);
		dw = duk_get_int(ctx, 3);
		dh = duk_get_int(ctx, 4);
	}
	else if (argc == 3) {
		sx = 0;
		sy = 0;
		sw = img->get_width();
		sh = img->get_height();
		dx = duk_get_int(ctx, 1);
		dy = duk_get_int(ctx, 2);
		dw = img->get_width();
		dh = img->get_height();
	}
	else {
		sx = 0;
		sy = 0;
		sw = img->get_width();
		sh = img->get_height();
		dx = 0;
		dy = 0;
		dw = img->get_width();
		dh = img->get_height();
	}
    thiz->bean->drawImage(img, sx, sy, sw, sh, dx, dy, dw, dh);
	return 0;
}
int AECanvasRenderingContext2D::fillRect(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	GLint x = duk_get_int(ctx, 0);
	GLint y = duk_get_int(ctx, 1);
	GLint w = duk_get_int(ctx, 2);
	GLint h = duk_get_int(ctx, 3);
    thiz->bean->fillRect(x, y, w, h);
	return 0;
}
int AECanvasRenderingContext2D::fillText(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	const char* s = duk_get_str(ctx, 0);
	const GLint x = duk_get_int(ctx, 1);
	const GLint y = duk_get_int(ctx, 2);
    thiz->bean->fillText(s, x, y);
	return 0;
}
int AECanvasRenderingContext2D::getImageData(duk_context* ctx) {
	js_thiz(AECanvasRenderingContext2D);
	uint x = duk_get_uint(ctx, 0);
	uint y = duk_get_uint(ctx, 1);
	uint w = duk_get_uint(ctx, 2);
	uint h = duk_get_uint(ctx, 3);

//	duk_eval_string(cx, "ImageData");
//	duk_pnew(cx, 0);
//	js_that(-1, AEImageData, data);
//	data->resize(w, h);
//	thiz->bean->getImageData(x, y, w, h, data->buffer);
//	return 1;

	uint width  = thiz->bean->getWidth();
	uint height = thiz->bean->getHeight();

	duk_push_object(ctx);											// object
	js_set_uint(p_width, width);									// object[width]  = width
	js_set_uint(p_height, height);									// object[height] = height
	GLvoid* data = duk_push_fixed_buffer(ctx, width * height * 4);	// object
	thiz->bean->getImageData(x, y, w, h, data);						// object
	duk_put_prop_string(ctx, -2, p_data);							// object[data] = data;
	return 1;														// object
}
int AECanvasRenderingContext2D::measureText(duk_context *ctx) {
	js_thiz(AECanvasRenderingContext2D);
	const char* text = duk_get_string(ctx, 0);
    const float size = thiz->bean->measureText(text);
	duk_push_object(ctx);
	js_set_uint(p_width, size);
	return 1;
}
int AECanvasRenderingContext2D::scale(duk_context *ctx) {
	js_thiz(AECanvasRenderingContext2D);
	GLfloat x = duk_get_number(ctx, 0);
	GLfloat y = duk_get_number(ctx, 1);
    thiz->bean->scale(x, y);
	return 0;
}
int AECanvasRenderingContext2D::strokeText(duk_context *ctx) {
	js_thiz(AECanvasRenderingContext2D);
	const char* s = duk_get_str(ctx, 0);
	const GLint x = duk_get_int(ctx, 1);
	const GLint y = duk_get_int(ctx, 2);
    thiz->bean->strokeText(s, x, y);
	return 0;
}

void AECanvasRenderingContext2D::binding(duk_context *ctx) {
	js_begin_binding();
    js_add_prop(font);
    js_add_prop(fillStyle);
    js_add_prop(lineWidth);
    js_add_prop(textBaseline);

	js_binding_func( 2, size); // canvas大小变化时，朗英会调用此接口
	js_binding_func( 4, clearRect);
	js_binding_func(-1, drawImage);
	js_binding_func( 4, fillRect);
	js_binding_func(-1, fillText);
	js_binding_func( 4, getImageData);
	js_binding_func( 1, measureText);
	js_binding_func( 2, scale);
	js_binding_func(-1, strokeText);
	js_end_binding(CanvasRenderingContext2D);
}
