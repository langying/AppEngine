//
//  AECanvasRenderingContext2D.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/1/12.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AECanvasRenderingContext2D_hpp
#define AECanvasRenderingContext2D_hpp

#include "AEManager.hpp"
#include "AECanvasRenderingContext2DBean.hpp"

class AECanvasRenderingContext2D : public AEImage {
public:
    js_define(font);
    js_define(fillStyle);
    js_define(lineWidth);
    js_define(textBaseline);
    js_prop(AECanvasRenderingContext2D, uint, width);
    js_prop(AECanvasRenderingContext2D, uint, height);
    js_con_destructor(AECanvasRenderingContext2D, [](duk_context* ctx, AECanvasRenderingContext2D* thiz, GLint argc) {
        thiz->init(AEManager::find(ctx));
    });
    
public:
	js_func(size); // canvas大小变化时，朗英会调用此接口
	js_func(scale);
	js_func(fillRect);
	js_func(fillText);
	js_func(clearRect);
	js_func(drawImage);
	js_func(strokeText);
	js_func(measureText);
	js_func(getImageData);

public:
	AECanvasRenderingContext2DBean* bean;
	void init(AEManager* mgr);
	void texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
	void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
};

#endif /* AECanvasRenderingContext2D_hpp */
