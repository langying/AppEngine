//
//  AEHTMLImageElementBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/9/28.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEHTMLImageElementBean_hpp
#define AEHTMLImageElementBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEHTMLImageElement;
class AEHTMLImageElementBean {
public:
    static std::map<AEHTMLImageElementBean*, int> beans;
    static void response(AEHTMLImageElementBean* texture, std::string url, GLuint width, GLuint height);
    
public:
    AEManager*  mgr;
    std::string src;
    AEHTMLImageElement* img;
    
public:
    ~AEHTMLImageElementBean();
    AEHTMLImageElementBean(AEHTMLImageElement* image);
    void init(AEManager* mgr);
    void setSrc(const char* url);
    void texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
};

#endif /* AEHTMLImageElementBean_hpp */
