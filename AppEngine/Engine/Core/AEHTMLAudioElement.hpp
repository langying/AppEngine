//
//  AEHTMLAudioElement.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEHTMLAudioElement_hpp
#define AEHTMLAudioElement_hpp

#include "AEHTMLAudioElementBean.hpp"

class AEHTMLAudioElement {
public:
    js_prop(AEHTMLAudioElement, uint,  duration);
    js_prop(AEHTMLAudioElement, uint,  readyState);
    js_prop(AEHTMLAudioElement, float, defaultPlaybackRate);
    
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
    
    js_con_destructor(AEHTMLAudioElement, [](duk_context* ctx, AEHTMLAudioElement* thiz, GLint argc) {
        thiz->init(ctx);
    });
    
public:
    GLuint ID;
    AEHTMLAudioElementBean* bean;
    void init(duk_context* ctx);
    void onFinish(GLuint width, GLuint height, GLuint duration);
};

#endif /* AEHTMLAudioElement_hpp */
