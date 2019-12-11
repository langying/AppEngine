//
//  AESound.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AESound_hpp
#define AESound_hpp

#include "AESoundBean.hpp"

class AESound {
public:
    js_define(muted);
    js_define(volume);

    js_func(load);
    js_func(play);
    js_func(stop);
    js_con_destructor(AESound, [](duk_context* ctx, AESound* thiz, GLint argc) {
        thiz->init(ctx);
    });
    
public:
    AESoundBean* bean;
    void init(duk_context* ctx);
};

#endif /* AESound_hpp */
