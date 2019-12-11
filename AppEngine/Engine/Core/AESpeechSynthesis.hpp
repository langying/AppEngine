//
//  AESpeechSynthesis.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/11.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AESpeechSynthesis_hpp
#define AESpeechSynthesis_hpp

#include "AESpeechSynthesisBean.hpp"

class AESpeechSynthesis {
public:
	js_func(pause);
	js_func(speak);
	js_func(cancel);
	js_func(resume);
	js_func(getVoices);

	js_define(paused);
	js_define(pending);
	js_define(speaking);

    js_con_destructor(AESpeechSynthesis, [](duk_context* ctx, AESpeechSynthesis* thiz, GLint argc) {
    	thiz->init(ctx);
    });

public:
    AESpeechSynthesisBean* bean;
    void init(duk_context* ctx);
};

#endif /* AESpeechSynthesis_hpp */
