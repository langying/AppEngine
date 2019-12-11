//
//  AEVRDisplay.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/8/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEVRDisplay_hpp
#define AEVRDisplay_hpp


#include "AEVRDisplayBean.hpp"

class AEVRDisplay {
public:
    js_prop(AEVRDisplay, float, x);
    js_prop(AEVRDisplay, float, y);
    js_prop(AEVRDisplay, float, z);
    js_prop(AEVRDisplay, float, w);
    js_prop(AEVRDisplay, float, lu);
    js_prop(AEVRDisplay, float, ld);
    js_prop(AEVRDisplay, float, ll);
    js_prop(AEVRDisplay, float, lr);
    js_prop(AEVRDisplay, float, ru);
    js_prop(AEVRDisplay, float, rd);
    js_prop(AEVRDisplay, float, rl);
    js_prop(AEVRDisplay, float, rr);
    js_prop(AEVRDisplay, float, depthFar);
    js_prop(AEVRDisplay, float, depthNear);
    js_prop(AEVRDisplay, bool,  isConnected);
    js_prop(AEVRDisplay, bool,  isPresenting);
    js_prop(AEVRDisplay, string, displayId);
    js_prop(AEVRDisplay, string, displayName);
    
    js_con_destructor(AEVRDisplay, [](duk_context* ctx, AEVRDisplay* thiz, GLint argc) {
        thiz->init(ctx);
    });
    
public:
    js_func(update);
    js_func(resetPose);
    js_func(submitFrame);
    
private:
    AEVRDisplayBean* bean;
    void init(duk_context* ctx);
};

#endif /* AEVRDisplay_hpp */
