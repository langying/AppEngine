//
//  AEHTMLAudioElementBean.mm
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import "AEURLCache.h"
#import "AEHTMLAudioElementImpl.h"

#import "AEManager.hpp"
#import "AEHTMLAudioElement.hpp"
#import "AEHTMLAudioElementBean.hpp"

AEHTMLAudioElementBean::~AEHTMLAudioElementBean() {
    [(__bridge AEHTMLAudioElementImpl*)impl release];
}
AEHTMLAudioElementBean::AEHTMLAudioElementBean(AEHTMLAudioElement* audio) {
    this->mgr   = nullptr;
    this->impl  = nullptr;
    this->audio = audio;
}

void AEHTMLAudioElementBean::init(AEManager* mgr) {
    this->mgr  = mgr;
    this->impl = (__bridge void*)[AEHTMLAudioElementImpl.alloc initWithAudioBean:this];
}

void AEHTMLAudioElementBean::load() {
}
void AEHTMLAudioElementBean::play() {
}
void AEHTMLAudioElementBean::stop() {
    [(__bridge AEHTMLAudioElementImpl*)impl stop];
}
void AEHTMLAudioElementBean::release() {
}

std::string AEHTMLAudioElementBean::getSrc() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).src.UTF8String;
}
void AEHTMLAudioElementBean::setSrc(const char* src_) {
    std::string src = mgr->absURL(src_);
    NSString*   url = [[NSString.alloc initWithUTF8String:src.c_str()] autorelease];
    ((__bridge AEHTMLAudioElementImpl*)impl).src = url;
}
bool AEHTMLAudioElementBean::getLoop() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).loop;
}
void AEHTMLAudioElementBean::setLoop(bool val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).loop = val;
}
bool AEHTMLAudioElementBean::getMuted() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).muted;
}
void AEHTMLAudioElementBean::setMuted(bool val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).muted = val;
}
bool AEHTMLAudioElementBean::getPaused() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).paused;
}
void AEHTMLAudioElementBean::setPaused(bool val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).paused = val;
}
bool AEHTMLAudioElementBean::getAutoplay() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).autoplay;
}
void AEHTMLAudioElementBean::setAutoplay(bool val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).autoplay = val;
}
uint AEHTMLAudioElementBean::getCurrentTime() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).currentTime;
}
void AEHTMLAudioElementBean::setCurrentTime(uint val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).currentTime = val;
}
float AEHTMLAudioElementBean::getVolume() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).volume;
}
void AEHTMLAudioElementBean::setVolume(float val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).volume = val;
}
float AEHTMLAudioElementBean::getPlaybackRate() {
    return ((__bridge AEHTMLAudioElementImpl*)impl).playbackRate;
}
void AEHTMLAudioElementBean::setPlaybackRate(float val) {
    ((__bridge AEHTMLAudioElementImpl*)impl).playbackRate = val;
}
