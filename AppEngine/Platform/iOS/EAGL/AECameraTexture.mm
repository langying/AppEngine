//
//  AECameraTexture.m
//  AppEngine
//
//  Created by 韩琼 on 2017/1/25.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import "AEURLCache.h"
#import "AEAGLContext.h"
#import "AECameraTexture.h"
#import "AEHTMLVideoElementImpl.h"
#import <AVFoundation/AVFoundation.h>

@interface AECameraTexture()<AVCaptureVideoDataOutputSampleBufferDelegate> {
    GLfloat _matrix[16];
}

@property(nonatomic, assign) BOOL    dirty;
@property(nonatomic, assign) GLuint  texId;
@property(nonatomic, assign) GLuint  width;
@property(nonatomic, assign) GLuint  height;
@property(nonatomic, assign) GLuint  duration;
@property(nonatomic, assign) AEHTMLVideoElementImpl* impl;

@property(nonatomic, retain) NSString*                 url;
@property(nonatomic, retain) AEAGLContext*             context;
@property(nonatomic, retain) AVCaptureSession*         session;
@property(nonatomic, retain) AVCaptureDeviceInput*     input;
@property(nonatomic, retain) AVCaptureVideoDataOutput* output;

@end

@implementation AECameraTexture

- (void)dealloc {
    [self stop];
    self.url = nil;
    self.input   = nil;
    self.output  = nil;
    self.context = nil;
    self.session = nil;
    [super dealloc];
}

- (instancetype)initWithVideo:(AEHTMLVideoElementImpl*)impl context:(EAGLContext*)context url:(NSString*)url {
    if ((self = [super init]) == nil) {
        return nil;
    }
    
    memset(_matrix, 0, sizeof(_matrix));
    CGAffineTransform t = CGAffineTransformScale(CGAffineTransformIdentity, 1, -1);
    _matrix[0]  = t.a;
    _matrix[4]  = t.b;
    _matrix[1]  = t.c;
    _matrix[5]  = t.d;
    _matrix[10] = 1;
    _matrix[15] = 1;
    
    self.url     = url;
    self.impl    = impl;
    self.context = [[AEAGLContext.alloc init] autorelease];
    self.input = ({
        AVCaptureDevice* camera = nil;
        NSArray* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
        for (AVCaptureDevice* device in devices) {
            if ([device position] == AVCaptureDevicePositionBack) {
                camera = device;
                break;
            }
        }
        [[AVCaptureDeviceInput.alloc initWithDevice:camera error:nil] autorelease];
    });
    self.output = ({
        AVCaptureVideoDataOutput* output = [[AVCaptureVideoDataOutput.alloc init] autorelease];
        output.alwaysDiscardsLateVideoFrames = YES;
        output.videoSettings = @{(id)kCVPixelBufferPixelFormatTypeKey : @(kCVPixelFormatType_32BGRA)};
        [output setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
        [[output connectionWithMediaType:AVMediaTypeVideo] setVideoOrientation:AVCaptureVideoOrientationPortrait];
        output;
    });
    self.session = ({
        AVCaptureSession* session = [[AVCaptureSession.alloc init] autorelease];
        session.sessionPreset = AVCaptureSessionPresetiFrame960x540;
        if ([session canAddInput:_input]) {
            [session addInput:_input];
        }
        if ([session canAddOutput:_output]) {
            [session addOutput:_output];
        }
        session;
    });
    [self.impl onPrepared:self width:960 height:540 duration:INT_MAX];
    return self;
}

- (void)captureOutput:(AVCaptureOutput*)output didOutputSampleBuffer:(CMSampleBufferRef)sample fromConnection:(AVCaptureConnection*)connection {
    _dirty = YES;
    if (_texId) {
        EAGLContext* cx = [EAGLContext currentContext];
        CVPixelBufferRef buffer = CMSampleBufferGetImageBuffer(sample);
        [_context drawTarget:_texId buffer:buffer matrix:_matrix];
        [EAGLContext setCurrentContext:cx];
        _texId = 0;
    }
}

- (BOOL)open {
    [_session startRunning];
    return true;
}

- (BOOL)isDirty {
    return _dirty;
}
- (NSString*)currentURL {
    return self.url;
}
- (void)play {
}
- (void)stop {
    [_session stopRunning];
}
- (void)close {
}
- (void)texImage2D:(GLuint)texId target:(GLenum)target level:(GLint)level infmt:(GLint)infmt format:(GLenum)format type:(GLenum)type {
    _dirty = NO;
    _texId = texId;
}

- (void)texSubImage2D:(GLenum)target level:(GLint)level xoffset:(GLint)xoffset yoffset:(GLint)yoffset format:(GLenum)format type:(GLenum)type {
}

// --------------------------Getter_And_Setter------------------
- (BOOL)loop {
    return NO;
}
- (void)setLoop:(BOOL)loop {
}
- (BOOL)muted {
    return NO;
}
- (void)setMuted:(BOOL)muted {
}
- (BOOL)paused {
    return NO;
}
- (void)setPaused:(BOOL)paused {
}
- (BOOL)autoplay {
    return YES;
}
- (void)setAutoplay:(BOOL)autoplay {
}
- (uint)currentTime {
    return 0;
}
- (void)setCurrentTime:(uint)currentTime {
}
- (float)volume {
    return 1;
}
- (void)setVolume:(float)volume {
}
- (float)playbackRate {
    return 1;
}
- (void)setPlaybackRate:(float)playbackRate {
}

@end
