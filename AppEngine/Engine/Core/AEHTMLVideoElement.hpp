//
//  HTMLVideoElement.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/8/23.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef HTMLVideoElement_hpp
#define HTMLVideoElement_hpp

#include "AEHTMLVideoElementBean.hpp"

class AEHTMLVideoElement: public AEImage {
public:
	js_prop(AEHTMLVideoElement, uint,  width);
	js_prop(AEHTMLVideoElement, uint,  height);
	js_prop(AEHTMLVideoElement, uint,  duration);
	js_prop(AEHTMLVideoElement, uint,  readyState);
	js_prop(AEHTMLVideoElement, float, defaultPlaybackRate);

    js_define(src);
	js_define(loop);
	js_define(muted);
	js_define(paused);
	js_define(volume);
	js_define(autoplay);
	js_define(currentTime);
	js_define(playbackRate);

	js_func(load);
	js_func(play);
	js_func(stop);

    js_con_destructor(AEHTMLVideoElement, [](duk_context* ctx, AEHTMLVideoElement* thiz, GLint argc) {
    	thiz->init(ctx);
    });

public:
    GLuint ID;
    AEHTMLVideoElementBean* bean;
    void init(duk_context* ctx);
    void onFinish(GLuint width, GLuint height, GLuint duration);
    void texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
};

#endif /* HTMLVideoElement_hpp */
