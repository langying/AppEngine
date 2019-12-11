//
//  AEHTMLVideoElementImpl.m
//  AppEngine
//
//  Created by 韩琼 on 2017/2/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import "AEURLCache.h"
#import "AEVideoTexture.h"
#import "AECameraTexture.h"
#import "AEHTMLVideoElement.hpp"
#import "AEHTMLVideoElementImpl.h"

#import <AVFoundation/AVFoundation.h>

@interface AEHTMLVideoElementImpl()

@property(nonatomic, retain) NSString*         url;
@property(nonatomic, retain) id<AEVideoStream> stream;
@property(nonatomic, assign) AEHTMLVideoElementBean* bean;

@end

@implementation AEHTMLVideoElementImpl

- (void)dealloc {
    [self stop];
    self.url    = nil;
    self.bean   = nil;
    self.stream = nil;
    [super dealloc];
}

- (instancetype)initWithVideoBean:(AEHTMLVideoElementBean*)bean {
    if ((self = [super init]) == nil) {
        return nil;
    }
    self.bean = bean;
    return self;
}

- (void)stop {
    [_stream stop];
}

- (void)onPrepared:(id<AEVideoStream>)stream width:(GLuint)width height:(GLuint)height duration:(GLuint)duration {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.bean) {
            self.bean->video->onFinish(width, height, duration);
        }
    });
}
- (void)texImage2D:(GLuint)texture target:(GLenum)target level:(GLint)level infmt:(GLint)infmt format:(GLenum)format type:(GLenum)type {
    if (![_stream isDirty]) {
        return;
    }
    [_stream texImage2D:texture target:target level:level infmt:infmt format:format type:type];
}

- (void)texSubImage2D:(GLenum)target level:(GLint)level xoffset:(GLint)xoffset yoffset:(GLint)yoffset format:(GLenum)format type:(GLenum)type {
    [_stream texSubImage2D:target level:level xoffset:xoffset yoffset:yoffset format:format type:type];
}

/** --------------------Getter_And_Setter_Methods------------------- */
- (NSString*)src {
    return self.url;
}
- (void)setSrc:(NSString*)src {
    self.url = src;
    EAGLContext* context = [EAGLContext currentContext];
    if ([src hasPrefix:@"camera://"]) {
        self.stream = [[AECameraTexture.alloc initWithVideo:self context:context url:_url] autorelease];
    }
    else {
        self.stream = [[AEVideoTexture.alloc initWithVideo:self context:context url:_url] autorelease];
    }
    [_stream open];
}
- (BOOL)loop {
    return _stream.loop;
}
- (void)setLoop:(BOOL)loop {
    _stream.loop = loop;
}
- (BOOL)muted {
    return _stream.muted;
}
- (void)setMuted:(BOOL)muted {
    _stream.muted = muted;
}
- (BOOL)paused {
    return _stream.paused;
}
- (void)setPaused:(BOOL)paused {
    _stream.paused = paused;
}
- (BOOL)autoplay {
    return _stream.autoplay;
}
- (void)setAutoplay:(BOOL)autoplay {
    _stream.autoplay = autoplay;
}
- (uint)currentTime {
    return _stream.currentTime;
}
- (void)setCurrentTime:(uint)currentTime {
    _stream.currentTime = currentTime;
}
- (float)volume {
    return _stream.volume;
}
- (void)setVolume:(float)volume {
    _stream.volume = volume;
}
- (float)playbackRate {
    return _stream.playbackRate;
}
- (void)setPlaybackRate:(float)playbackRate {
    _stream.playbackRate = playbackRate;
}

@end
