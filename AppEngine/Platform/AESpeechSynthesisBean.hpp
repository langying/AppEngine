//
//  AESpeechSynthesisBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/11.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AESpeechSynthesisBean_hpp
#define AESpeechSynthesisBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AESpeechSynthesis;
class AESpeechSynthesisBean {
public:
	~AESpeechSynthesisBean();
	AESpeechSynthesisBean(AESpeechSynthesis* that);

public:
	void pause();
	void speak(float rate, const char* lang, const char* text);
	void cancel();
	void resume();
	void getVoices();

	bool paused();
	bool pending();
	bool speaking();

public:
	jobject impl;
	AEManager* manager;
	AESpeechSynthesis* that;
	void init(AEManager* mgr);
	void release();
};

#endif /* AESpeechSynthesisBean_hpp */
