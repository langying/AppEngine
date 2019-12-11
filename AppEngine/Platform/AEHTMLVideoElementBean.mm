//
//  AEHTMLVideoElementBean.mm
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#import "AEURLCache.h"
#import "AEHTMLVideoElementImpl.h"

#import "AEManager.hpp"
#import "AEHTMLVideoElement.hpp"
#import "AEHTMLVideoElementBean.hpp"

AEHTMLVideoElementBean::~AEHTMLVideoElementBean() {
    [(__bridge AEHTMLVideoElementImpl*)impl release];
}
AEHTMLVideoElementBean::AEHTMLVideoElementBean(AEHTMLVideoElement* video) {
    this->mgr   = nullptr;
    this->impl  = nullptr;
    this->video = video;
}

void AEHTMLVideoElementBean::init(AEManager* mgr) {
    this->mgr  = mgr;
    this->impl = (__bridge void*)[AEHTMLVideoElementImpl.alloc initWithVideoBean:this];
}

void AEHTMLVideoElementBean::load() {
}
void AEHTMLVideoElementBean::play() {
}
void AEHTMLVideoElementBean::stop() {
    [(__bridge AEHTMLVideoElementImpl*)impl stop];
}
void AEHTMLVideoElementBean::release() {
}
void AEHTMLVideoElementBean::texImage2D(GLuint texture, GLenum target, GLint level, GLint infmt, GLenum format, GLenum type) {
    [(__bridge AEHTMLVideoElementImpl*)impl texImage2D:texture target:target level:level infmt:infmt format:format type:type];
}
void AEHTMLVideoElementBean::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type) {
    [(__bridge AEHTMLVideoElementImpl*)impl texSubImage2D:target level:level xoffset:xoffset yoffset:yoffset format:format type:type];
}

std::string AEHTMLVideoElementBean::getSrc() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).src.UTF8String;
}
void AEHTMLVideoElementBean::setSrc(const char* src_) {
    std::string src = mgr->absURL(src_);
    NSString*   url = [[NSString.alloc initWithUTF8String:src.c_str()] autorelease];
    ((__bridge AEHTMLVideoElementImpl*)impl).src = url;
}
bool AEHTMLVideoElementBean::getLoop() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).loop;
}
void AEHTMLVideoElementBean::setLoop(bool val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).loop = val;
}
bool AEHTMLVideoElementBean::getMuted() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).muted;
}
void AEHTMLVideoElementBean::setMuted(bool val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).muted = val;
}
bool AEHTMLVideoElementBean::getPaused() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).paused;
}
void AEHTMLVideoElementBean::setPaused(bool val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).paused = val;
}
bool AEHTMLVideoElementBean::getAutoplay() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).autoplay;
}
void AEHTMLVideoElementBean::setAutoplay(bool val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).autoplay = val;
}
uint AEHTMLVideoElementBean::getCurrentTime() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).currentTime;
}
void AEHTMLVideoElementBean::setCurrentTime(uint val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).currentTime = val;
}
float AEHTMLVideoElementBean::getVolume() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).volume;
}
void AEHTMLVideoElementBean::setVolume(float val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).volume = val;
}
float AEHTMLVideoElementBean::getPlaybackRate() {
    return ((__bridge AEHTMLVideoElementImpl*)impl).playbackRate;
}
void AEHTMLVideoElementBean::setPlaybackRate(float val) {
    ((__bridge AEHTMLVideoElementImpl*)impl).playbackRate = val;
}
