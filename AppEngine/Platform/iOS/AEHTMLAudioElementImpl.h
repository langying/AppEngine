//
//  AEHTMLAudioElementImpl.h
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AEHTMLAudioElementBean.hpp"

@interface AEHTMLAudioElementImpl : NSObject

@property(nonatomic, retain) NSString* src;

@property(nonatomic, assign) BOOL  loop;
@property(nonatomic, assign) BOOL  muted;
@property(nonatomic, assign) BOOL  paused;
@property(nonatomic, assign) BOOL  autoplay;
@property(nonatomic, assign) uint  currentTime;
@property(nonatomic, assign) float volume;
@property(nonatomic, assign) float playbackRate;

- (instancetype)initWithAudioBean:(AEHTMLAudioElementBean*)bean;

- (void)stop;
- (void)onPrepared:(id)stream width:(GLuint)width height:(GLuint)height duration:(GLuint)duration;

@end
