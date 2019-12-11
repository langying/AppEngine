//
//  AEHTMLAudioElementImpl.m
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import "AEURLCache.h"
#import "AEVideoTexture.h"
#import "AECameraTexture.h"
#import "AEHTMLAudioElement.hpp"
#import "AEHTMLAudioElementImpl.h"

#import <AVFoundation/AVFoundation.h>

@interface AEHTMLAudioElementImpl()

@property(nonatomic, retain) id stream;
@property(nonatomic, retain) NSString* url;
@property(nonatomic, assign) AEHTMLAudioElementBean* bean;

@end

@implementation AEHTMLAudioElementImpl

- (void)dealloc {
    [self stop];
    self.url    = nil;
    self.bean   = nil;
    self.stream = nil;
    [super dealloc];
}

- (instancetype)initWithAudioBean:(AEHTMLAudioElementBean*)bean {
    if ((self = [super init]) == nil) {
        return nil;
    }
    self.bean = bean;
    return self;
}

- (void)stop {
    [_stream stop];
}

- (void)onPrepared:(id)stream width:(GLuint)width height:(GLuint)height duration:(GLuint)duration {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.bean) {
            self.bean->audio->onFinish(width, height, duration);
        }
    });
}

@end
