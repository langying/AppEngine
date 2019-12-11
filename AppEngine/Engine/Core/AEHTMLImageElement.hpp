//
//  AEHTMLImageElement.hpp
//  Action
//
//  Created by 韩琼 on 16/1/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEHTMLImageElement_hpp
#define AEHTMLImageElement_hpp

#include "AEManager.hpp"
#include "AEHTMLImageElementBean.hpp"

class AEHTMLImageElement : public AEImage {
public:
    js_define(src);
    js_prop(AEHTMLImageElement, uint, width);
    js_prop(AEHTMLImageElement, uint, height);
    js_prop(AEHTMLImageElement, bool, complete);
    js_con_destructor(AEHTMLImageElement, [](duk_context* ctx, AEHTMLImageElement* thiz, GLint argc) {
    	thiz->init(AEManager::find(ctx));
    });
    
public:
    GLuint ID;
    AEHTMLImageElementBean* bean;
    void init(AEManager* mgr);
    void onFinish(GLuint width, GLuint height);
    void texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    std::string pathfile();
};

#endif /* AEHTMLImageElement_hpp */
