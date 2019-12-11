//
//  AEHTMLVideoElementBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEHTMLVideoElementBean_hpp
#define AEHTMLVideoElementBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEHTMLVideoElement;
class AEHTMLVideoElementBean {
public:
    ~AEHTMLVideoElementBean();
    AEHTMLVideoElementBean(AEHTMLVideoElement* video);
    
    void load();
    void play();
    void stop();
    void release();
    void texImage2D(GLuint texture, GLenum target, GLint level, GLint infmt, GLenum format, GLenum type);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type);
    
    std::string getSrc();	void setSrc(const char* src);
    bool  getLoop();		void setLoop(bool val);
    bool  getMuted();		void setMuted(bool val);
    bool  getPaused();		void setPaused(bool val);
    bool  getAutoplay();  	void setAutoplay(bool val);
    uint  getCurrentTime(); void setCurrentTime(uint val);
    float getVolume();		void setVolume(float val);
    float getPlaybackRate();void setPlaybackRate(float val);
    
public:
    jobject impl;
    AEManager* mgr;
    AEHTMLVideoElement* video;
    void init(AEManager* mgr);
};

#endif /* AEHTMLVideoElementBean_hpp */
