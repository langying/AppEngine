//
//  AEVideoTexture.m
//  AppEngine
//
//  Created by 韩琼 on 2017/1/25.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import "AEURLCache.h"
#import "AEAGLContext.h"
#import "AEVideoTexture.h"
#import "AEHTMLVideoElementImpl.h"
#import <AVFoundation/AVFoundation.h>

#define ONE_FRAME_DURATION 0.03

NSString* kTracks             = @"tracks";
NSString* kStatus             = @"player.currentItem.status";
NSString* kPreferredTransform = @"preferredTransform";

@interface AEVideoTexture()<AVPlayerItemOutputPullDelegate> {
    BOOL  _loop;
    BOOL  _autoplay;
    float _volume;
    GLfloat _matrix[16];
}

@property(nonatomic, assign) BOOL   dirty;
@property(nonatomic, assign) GLuint width;
@property(nonatomic, assign) GLuint height;
@property(nonatomic, assign) GLuint duration;
@property(nonatomic, assign) AEHTMLVideoElementImpl* impl;

@property(nonatomic, retain) AVPlayer*                player;
@property(nonatomic, retain) AEAGLContext*            context;
@property(nonatomic, retain) AVPlayerItemVideoOutput* output;

@end

@implementation AEVideoTexture

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
    [self removeObserver:self forKeyPath:kStatus];
    [self.player replaceCurrentItemWithPlayerItem:nil];
    
    self.url     = nil;
    self.player  = nil;
    self.context = nil;
    self.output  = nil;
    [super dealloc];
}

- (instancetype)initWithVideo:(AEHTMLVideoElementImpl*)impl context:(EAGLContext*)context url:(NSString*)url {
    if ((self = [super init]) == nil) {
        return nil;
    }
    
    _loop     = NO;
    _autoplay = YES;
    _volume   = 1.0f;
    memset(_matrix, 0, sizeof(_matrix));
    _matrix[0] = _matrix[5] = _matrix[10] = _matrix[15] = 1;
    
    self.url     = url;
    self.impl    = impl;
    self.player  = [[AVPlayer.alloc init] autorelease];
    self.context = [[AEAGLContext.alloc init] autorelease];
    
    NSString* k1 = (__bridge NSString*)kCVPixelBufferPixelFormatTypeKey;
    NSNumber* v1 = @(kCVPixelFormatType_32BGRA);
    NSString* k2 = (__bridge NSString*)kCVPixelBufferBytesPerRowAlignmentKey;
    NSNumber* v2 = @1;
    NSString* k3 = (__bridge NSString*)kCVPixelBufferOpenGLESCompatibilityKey;
    NSNumber* v3 = @YES;
    self.output = [[AVPlayerItemVideoOutput.alloc initWithPixelBufferAttributes:@{k1:v1, k2:v2, k3:v3}] autorelease];
    [_output setDelegate:self queue:nil];
    [_output requestNotificationOfMediaDataChangeWithAdvanceInterval:1];
    
    [self addObserver:self forKeyPath:kStatus options:NSKeyValueObservingOptionNew context:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(onPlayEnd) name:AVPlayerItemDidPlayToEndTimeNotification object:nil];
    return self;
}

- (void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary<NSKeyValueChangeKey, id>*)change context:(void*)context {
    switch (_player.currentItem.status) {
        case AVPlayerItemStatusReadyToPlay: {
            [_player play];
            
            AVAsset*      asset = _player.currentItem.asset;
            AVAssetTrack* track = [asset tracksWithMediaType:AVMediaTypeVideo].firstObject;
            CGAffineTransform t = CGAffineTransformScale([track preferredTransform], 1, -1);
            CMTime time = [asset duration];
            CGSize size = [track naturalSize];
            
            _matrix[0] = t.a;
            _matrix[4] = t.b;
            _matrix[1] = t.c;
            _matrix[5] = t.d;
            _width     = static_cast<GLuint>(size.width);
            _height    = static_cast<GLuint>(size.height);
            _duration  = static_cast<GLuint>(CMTimeGetSeconds(time) * 1000);
            [self.impl onPrepared:self width:_width height:_height duration:_duration];
            break;
        }
        case AVPlayerItemStatusFailed: {
            NSError* error = _player.currentItem.error;
            LOG("asset.status is failed, error is %s.", [error description].UTF8String);
            break;
        }
        case AVPlayerItemStatusUnknown: {
            NSError* error = _player.currentItem.error;
            LOG("asset.status is unknown, error is %s.", [error description].UTF8String);
            break;
        }
        default: {
            NSError* error = _player.currentItem.error;
            LOG("asset.status is default????, error is %s.", [error description].UTF8String);
            break;
            break;
        }
    }
}

- (void)outputMediaDataWillChange:(AVPlayerItemOutput*)sender {
}

- (void)outputSequenceWasFlushed:(AVPlayerItemOutput*)output {
    _dirty = true;
}

- (void)onPlayEnd {
    if (_loop) {
        [self.player seekToTime:kCMTimeZero];
        [self.player play];
    }
}

- (BOOL)open {
    [_player.currentItem removeOutput:_output];
    [_player setActionAtItemEnd:AVPlayerActionAtItemEndNone];
    [_player replaceCurrentItemWithPlayerItem:({
        AVPlayerItem* item = [AVPlayerItem playerItemWithURL:[NSURL URLWithString:_url]];
        [item addOutput:_output];
        item;
    })];
    [_player play];
    return true;
}

- (BOOL)isDirty {
    return [_output hasNewPixelBufferForItemTime:_player.currentItem.currentTime];
}
- (NSString*)currentURL {
    return self.url;
}

- (void)play {
}
- (void)stop {
    
}
- (void)close {
    
}
- (void)texImage2D:(GLuint)texId target:(GLenum)target level:(GLint)level infmt:(GLint)infmt format:(GLenum)format type:(GLenum)type {
    EAGLContext* cx = [EAGLContext currentContext];
    CVPixelBufferRef buffer = [_output copyPixelBufferForItemTime:_player.currentItem.currentTime itemTimeForDisplay:nil];    
    [_context drawTarget:texId buffer:buffer matrix:_matrix];
    CVPixelBufferRelease(buffer);
    [EAGLContext setCurrentContext:cx];
}

- (void)texSubImage2D:(GLenum)target level:(GLint)level xoffset:(GLint)xoffset yoffset:(GLint)yoffset format:(GLenum)format type:(GLenum)type {
}

// --------------------------Getter_And_Setter------------------
- (BOOL)loop {
    return _loop;
}
- (void)setLoop:(BOOL)loop {
    _loop = loop;
}
- (BOOL)muted {
    return _player.muted;
}
- (void)setMuted:(BOOL)muted {
    _player.muted = muted;
}
- (BOOL)paused {
    return _player.timeControlStatus == AVPlayerTimeControlStatusPaused;
}
- (void)setPaused:(BOOL)paused {
    if (paused) {
         [_player pause];
    }
    else {
        [_player play];
    }
}
- (BOOL)autoplay {
    return _autoplay;
}
- (void)setAutoplay:(BOOL)autoplay {
    _autoplay = autoplay;
}
- (uint)currentTime {
    CMTime time = [_player currentTime];
    return static_cast<uint>(time.value / time.timescale);
}
- (void)setCurrentTime:(uint)currentTime {
    [_player seekToTime:CMTimeMake(currentTime, 1)];
}
- (float)volume {
    return _player.volume;
}
- (void)setVolume:(float)volume {
    _player.volume = volume;
}
- (float)playbackRate {
    return _player.rate;
}
- (void)setPlaybackRate:(float)playbackRate {
    _player.rate = playbackRate;
}

@end
