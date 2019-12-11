//
//  AEImageData.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEImageData_hpp
#define AEImageData_hpp

#include "AEPlatform.hpp"

class AEImageData : public AEImage {
public:
	void* buffer;
	js_define(data);
    js_prop(AEImageData, uint, width);
    js_prop(AEImageData, uint, height);
    js_con_destructor(AEImageData, [](duk_context* ctx, AEImageData* thiz, GLint argc) {
    });

public:
    void resize(GLuint width, GLuint height);
    void texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
};

#endif /* AEImageData_hpp */
