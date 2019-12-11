//
//  AESoundBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AESoundBean_hpp
#define AESoundBean_hpp

#include "AEPlatform.hpp"

class AESound;
class AEManager;
class AESoundBean {
public:
	bool  muted;
	float volume;
	jobject 	bean;
	AESound*    sound;
	AEManager* 	manager;

public:
	~AESoundBean();
	AESoundBean(AESound* sud);
    void init(AEManager* mgr);
    void release();
    void onBackground();
    void onForeground();

public:
	void vibrate();
    void stopALL();
    void pauseALL();
    void resumeALL();

public:
    void setMuted(bool muted);		bool  getMuted()  { return muted; };
    void setVolume(bool volume);	float getVolume() { return volume; }

public:
    uint play(const char* url);
    void stop(const char* url);
    void pause(const char* url);
    void resume(const char* url);
    void unload(const char* url);
    uint preload(const char* url);
};

#endif /* AESoundBean_hpp */
