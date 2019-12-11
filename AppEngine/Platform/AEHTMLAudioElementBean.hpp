//
//  AEHTMLAudioElementBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEHTMLAudioElementBean_hpp
#define AEHTMLAudioElementBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEHTMLAudioElement;
class AEHTMLAudioElementBean {
public:
    ~AEHTMLAudioElementBean();
    AEHTMLAudioElementBean(AEHTMLAudioElement* image);
    
    void load();
    void play();
    void stop();
    void release();
    
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
    AEHTMLAudioElement* audio;
    void init(AEManager* mgr);
};

#endif /* AEHTMLAudioElementBean_hpp */
