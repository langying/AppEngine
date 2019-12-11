//
//  AEHTMLVideoElementImpl.h
//  AppEngine
//
//  Created by 韩琼 on 2017/2/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AEHTMLVideoElementBean.hpp"

@protocol AEVideoStream <NSObject>

@required
@property(nonatomic, assign) BOOL  loop;
@property(nonatomic, assign) BOOL  muted;
@property(nonatomic, assign) BOOL  paused;
@property(nonatomic, assign) BOOL  autoplay;
@property(nonatomic, assign) uint  currentTime;
@property(nonatomic, assign) float volume;
@property(nonatomic, assign) float playbackRate;

- (BOOL)open;
- (BOOL)isDirty;
- (NSString*)currentURL;
- (void)play;
- (void)stop;
- (void)close;
- (void)texImage2D:(GLuint)texId target:(GLenum)target level:(GLint)level infmt:(GLint)infmt format:(GLenum)format type:(GLenum)type;
- (void)texSubImage2D:(GLenum)target level:(GLint)level xoffset:(GLint)xoffset yoffset:(GLint)yoffset format:(GLenum)format type:(GLenum)type;

@end

@interface AEHTMLVideoElementImpl : NSObject

@property(nonatomic, retain) NSString* src;

@property(nonatomic, assign) BOOL  loop;
@property(nonatomic, assign) BOOL  muted;
@property(nonatomic, assign) BOOL  paused;
@property(nonatomic, assign) BOOL  autoplay;
@property(nonatomic, assign) uint  currentTime;
@property(nonatomic, assign) float volume;
@property(nonatomic, assign) float playbackRate;

- (instancetype)initWithVideoBean:(AEHTMLVideoElementBean*)bean;

- (void)stop;
- (void)onPrepared:(id<AEVideoStream>)stream width:(GLuint)width height:(GLuint)height duration:(GLuint)duration;
- (void)texImage2D:(GLuint)texture target:(GLenum)target level:(GLint)level infmt:(GLint)infmt format:(GLenum)format type:(GLenum)type;
- (void)texSubImage2D:(GLenum)target level:(GLint)level xoffset:(GLint)xoffset yoffset:(GLint)yoffset format:(GLenum)format type:(GLenum)type;

@end
