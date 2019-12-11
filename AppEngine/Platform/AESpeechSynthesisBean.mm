//
//  AESpeechSynthesisBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/11.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#import "AEManager.hpp"
#import "AESpeechSynthesisBean.hpp"

AESpeechSynthesisBean::~AESpeechSynthesisBean() {
    release();
}
AESpeechSynthesisBean::AESpeechSynthesisBean(AESpeechSynthesis* that) {
    this->impl = nullptr;
    this->that = that;
    this->manager = nullptr;
}
void AESpeechSynthesisBean::init(AEManager* mgr) {
    this->manager = mgr;
}
void AESpeechSynthesisBean::release() {
}

void AESpeechSynthesisBean::pause() {
}
void AESpeechSynthesisBean::speak(float rate, const char* lang_, const char* text_) {
}
void AESpeechSynthesisBean::cancel() {
}
void AESpeechSynthesisBean::resume() {
}
void AESpeechSynthesisBean::getVoices() {
}

bool AESpeechSynthesisBean::paused() {
    return false;
}
bool AESpeechSynthesisBean::pending() {
    return false;
}
bool AESpeechSynthesisBean::speaking() {
    return false;
}
