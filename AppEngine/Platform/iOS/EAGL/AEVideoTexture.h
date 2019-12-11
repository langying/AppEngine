//
//  AEVideoTexture.h
//  AppEngine
//
//  Created by 韩琼 on 2017/1/25.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import <GLKit/GLKit.h>
#import "AEHTMLVideoElementImpl.h"

@interface AEVideoTexture : NSObject <AEVideoStream>

@property(nonatomic, retain) NSString* url;

- (instancetype)initWithVideo:(AEHTMLVideoElementImpl*)impl context:(EAGLContext*)context url:(NSString*)url;

@end
