//
//  AEView.h
//  Action
//
//  Created by 韩琼 on 16/1/12.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#import <GLKit/GLKit.h>

@class AEView;
@protocol AEViewDelegate <NSObject>
- (void)view:(AEView*)view fps:(NSInteger)fps;
- (NSString*)view:(AEView*)view handle:(NSString*)text;
@end


@interface AEView : GLKView

@property(nonatomic, assign) id<AEViewDelegate> handle;

- (void)loadURL:(NSString*)url;
- (NSString*)evalute:(NSString*)script;

@end
