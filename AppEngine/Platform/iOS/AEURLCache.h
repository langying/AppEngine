//
//  AEURLCache.h
//  AppEngine
//
//  Created by 韩琼 on 16/5/18.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AEURLCache : NSObject

+ (NSString*)textWithURL:(NSString*)name;
+ (NSString*)md5WithString:(NSString*)name;
+ (NSString*)absoluteString:(NSString*)name;
+ (NSString*)cachedFileWithURL:(NSString*)name;

@end
