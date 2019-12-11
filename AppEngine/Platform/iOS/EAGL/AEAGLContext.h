//
//  AEAGLContext.h
//  AppEngine
//
//  Created by 韩琼 on 2017/2/3.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import <CoreVideo/CoreVideo.h>

@interface AEAGLContext : NSObject

- (void)makeCurrent;
- (void)drawTarget:(GLuint)texId buffer:(CVPixelBufferRef)buffer matrix:(GLfloat*)matrix;

@end
