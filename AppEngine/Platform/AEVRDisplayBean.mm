//
//  AEVRDisplayBean.mm
//  AppEngine
//
//  Created by 韩琼 on 2016/12/3.
//  Copyright © 2016年 AppEngine. All rights reserved.
//


#include "AEManager.hpp"
#include "AEVRDisplay.hpp"
#include "AEVRDisplayBean.hpp"
#include "AEVRDisplayImpl.hpp"

//void* retain(CMMotionManager* mgr) {
//    return (__bridge_retained void*)mgr;
//}
//CMMotionManager* manager(void* mgr) {
//    return (__bridge CMMotionManager*)mgr;
//}
//CMMotionManager* release(void* mgr) {
//    return (__bridge_transfer CMMotionManager*)mgr;
//}

AEVRDisplayBean::~AEVRDisplayBean() {
    stop();
    delete (AEVRDisplayImpl*)impl;
}
AEVRDisplayBean::AEVRDisplayBean(AEVRDisplay* display) {
    this->impl    = new AEVRDisplayImpl();
    this->manager = nullptr;
    this->display = display;
}
void AEVRDisplayBean::init(AEManager* mgr) {
    this->manager = mgr;
}
void AEVRDisplayBean::stop() {
    ((AEVRDisplayImpl*)impl)->stopTracking();
}
void AEVRDisplayBean::start() {
    ((AEVRDisplayImpl*)impl)->startTracking();
}
void AEVRDisplayBean::update() {
    ((AEVRDisplayImpl*)impl)->update([this](GLKQuaternion* q, FieldOfView* leye, FieldOfView* reye) {
        display->x  = q->x;
        display->y  = q->y;
        display->z  = q->z;
        display->w  = q->w;
        display->lu = leye->getTop();
        display->ld = leye->getBottom();
        display->ll = leye->getLeft();
        display->lr = leye->getRight();
        display->ru = reye->getTop();
        display->rd = reye->getBottom();
        display->rl = reye->getLeft();
        display->rr = reye->getRight();
    });
}
