//
//  AEVRDisplay.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/8/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEManager.hpp"
#include "AEVRDisplay.hpp"

AEVRDisplay::~AEVRDisplay() {
    bean->stop();
    delete bean;
}
AEVRDisplay::AEVRDisplay(): cx(nullptr) {
	x = y = z = 0;
	w = 1;
    lu = ld = ll = lr = 0;
    ru = rd = rl = rr = 0;
    depthFar  = 100.0;
    depthNear = 0.01;
    isConnected = isPresenting = false;
    bean = new AEVRDisplayBean(this);
}
void AEVRDisplay::init(duk_context* ctx) {
	bean->init(AEManager::find(ctx));
	bean->start();
}
int AEVRDisplay::update(duk_context* ctx) {
    js_thiz(AEVRDisplay);
    thiz->bean->update();
    return 0;
}
int AEVRDisplay::resetPose(duk_context* ctx) {
    return 0;
}
int AEVRDisplay::submitFrame(duk_context* ctx) {
    return 0;
}

void AEVRDisplay::binding(duk_context* ctx) {
    js_begin_binding();

    js_add_prop(x);
    js_add_prop(y);
    js_add_prop(z);
    js_add_prop(w);
    js_add_prop(lu);
    js_add_prop(ld);
    js_add_prop(ll);
    js_add_prop(lr);
    js_add_prop(ru);
    js_add_prop(rd);
    js_add_prop(rl);
    js_add_prop(rr);
    js_add_prop(depthFar);
    js_add_prop(depthNear);
    js_add_prop(isConnected);
    js_add_prop(isPresenting);
    js_add_prop(displayId);
    js_add_prop(displayName);

    js_binding_func(0, update);
    js_binding_func(0, resetPose);
    js_binding_func(0, submitFrame);

    js_end_binding(VRDisplay);
}
