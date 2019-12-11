//
//  AEView.m
//  Action
//
//  Created by 韩琼 on 16/1/12.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#import "AEView.h"
#import "AEManager.hpp"

std::map<AEManager*, GLvoid*> _map;
std::string AEViewHandle(AEManager* mgr, const char* text) {
    if (!text) {
        return "";
    }
    auto  it = _map.find(mgr);
    void* pp = it == _map.end() ? nullptr : it->second;
    if (pp == nullptr) {
        return "";
    }
    else {
        AEView* gview = (__bridge AEView*)pp;
        NSString* ret = [gview.handle view:gview handle:[NSString stringWithUTF8String:text]];
        return ret ? ret.UTF8String : "";
    }
}

@interface AEView()<GLKViewDelegate>

@property(nonatomic, assign) CGFloat    num;
@property(nonatomic, assign) AEManager* mgr;

@end

@implementation AEView

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
    _map.erase(_mgr);
    delete _mgr;
    [super dealloc];
}

- (instancetype)initWithFrame:(CGRect)frame {
    EAGLContext* context = [[EAGLContext.alloc initWithAPI:kEAGLRenderingAPIOpenGLES2] autorelease];
    [EAGLContext setCurrentContext:context];
    
    if ((self = [super initWithFrame:frame context:context]) == nil) {
        return nil;
    }
    self.opaque                 = YES;
    self.delegate               = self;
    self.multipleTouchEnabled   = YES;
    self.enableSetNeedsDisplay  = NO;
    self.userInteractionEnabled = YES;
    self.drawableMultisample    = GLKViewDrawableMultisampleNone;
    self.drawableDepthFormat    = GLKViewDrawableDepthFormat24;
    self.drawableStencilFormat  = GLKViewDrawableStencilFormat8;
    self.drawableColorFormat    = GLKViewDrawableColorFormatRGBA8888;
    [self bindDrawable];
    
    CGFloat w = frame.size.width;
    CGFloat h = frame.size.height;
    UIDevice* dev = [UIDevice currentDevice];
    NSString* usr = [NSString stringWithFormat:@"%@/1.0 (%@; %@ %@)", @"Mobile", @"duck", dev.model, dev.systemVersion];
    self.num = [UIScreen mainScreen].scale;
    self.mgr = new AEManager(w*_num, h*_num, 1, usr.UTF8String);
    self.mgr->init();
    self.mgr->setHandle(AEViewHandle);
    _map.insert(std::make_pair(_mgr, (__bridge void*)self));
    
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(jsgc) name:UIApplicationWillTerminateNotification object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(jsgc) name:UIApplicationDidEnterBackgroundNotification object:nil];
    [NSNotificationCenter.defaultCenter addObserver:self selector:@selector(jsgc) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
    return self;
}
- (void)jsgc {
    _mgr->jsgc();
}
- (void)loadURL:(NSString*)url {
    _mgr->loadURL(url.UTF8String);
}
- (NSString*)evalute:(NSString*)script {
    std::string ret = _mgr->evalute(script.UTF8String);
    return [NSString stringWithUTF8String:ret.c_str()];
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event {
    [self touches:touches type:AEEvent::TouchStart];
}
- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event {
    [self touches:touches type:AEEvent::TouchMove];
}
- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event {
    [self touches:touches type:AEEvent::TouchEnd];
}
- (void)touchesCancelled:(NSSet*)touches withEvent:(UIEvent*)event {
    [self touches:touches type:AEEvent::TouchCancel];
}
- (void)touches:(NSSet*)touches type:(AEEvent::Type)type {
    GLint idx = 0;
    GLint len = static_cast<GLint>(touches.count);
    AETouch* pts = (AETouch*)malloc(len * sizeof(AETouch));
    for (UITouch* touch in touches) {
        CGPoint pt = [touch locationInView:touch.view];
        pts[idx].ID = reinterpret_cast<Integer>(touch);
        pts[idx].x  = pt.x * _num;
        pts[idx].y  = pt.y * _num;
        idx++;
    };
    _mgr->touch(type, len, pts);
    free(pts);
}

#pragma mark - GLKViewDelegate
- (void)glkView:(GLKView*)view drawInRect:(CGRect)rect {
    [self fps];
    _mgr->applyTimer();
}

#pragma mark - 渲染循环
- (void)fps {
    if (!_handle) {
        return;
    }
    static GLuint FPS = 0;
    static NSTimeInterval LastFpsTime = 0;
    
    FPS++;
    NSTimeInterval time = [NSDate.date timeIntervalSince1970];
    if (time - LastFpsTime >= 1.0f) {
        [_handle view:self fps:FPS];
        FPS = 0;
        LastFpsTime = time;
    }
}
@end
