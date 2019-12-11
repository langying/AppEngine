//
//  AESoundBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/8.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AESoundBean.hpp"

AESoundBean::~AESoundBean() {
    release();
}
AESoundBean::AESoundBean(AESound* sud) {
    muted	= false;
    volume	= 1;
    bean	= nullptr;
    sound	= sud;
    manager	= nullptr;
}
void AESoundBean::init(AEManager* mgr) {
    manager = mgr;
}
void AESoundBean::release() {
}
void AESoundBean::onBackground() {
}
void AESoundBean::onForeground() {
}
void AESoundBean::vibrate() {
}
void AESoundBean::stopALL() {
}
void AESoundBean::pauseALL() {
}
void AESoundBean::resumeALL() {
}
void AESoundBean::setMuted(bool muted) {
}
void AESoundBean::setVolume(bool volume) {
}
uint AESoundBean::play(const char* url_) {
    return 0;
}
void AESoundBean::stop(const char* url) {
}
void AESoundBean::pause(const char* url) {
}
void AESoundBean::resume(const char* url) {
}
void AESoundBean::unload(const char* url) {
}
uint AESoundBean::preload(const char* url) {
    return 0;
}
