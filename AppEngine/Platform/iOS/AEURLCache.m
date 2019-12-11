//
//  AEURLCache.m
//  AppEngine
//
//  Created by 韩琼 on 16/5/18.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#import "AEURLCache.h"
#import <CommonCrypto/CommonCrypto.h>

@implementation AEURLCache

+ (NSString*)textWithURL:(NSString*)name {
    NSString* url = [AEURLCache absoluteString:name];
    NSURLRequest* request = [NSURLRequest requestWithURL:[NSURL URLWithString:url] cachePolicy:NSURLRequestUseProtocolCachePolicy timeoutInterval:3.0];
    NSData* data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];
    return [[NSString.alloc initWithData:data encoding:NSUTF8StringEncoding] autorelease];
}

+ (NSString*)absoluteString:(NSString*)name {
    if ([name hasPrefix:@"/"]) {
        return [@"file://" stringByAppendingString:name];
    }
    else if ([name hasPrefix:@"file:"]) {
        if ([name hasPrefix:@"file:///asset/"]) {
            name = [name stringByReplacingOccurrencesOfString:@"file:///asset/" withString:@""];
            name = [[NSBundle.mainBundle resourcePath] stringByAppendingPathComponent:name];
        }
        else if([name hasPrefix:@"file:///cache/"]) {
            name = [name stringByReplacingOccurrencesOfString:@"file:///cache/" withString:@""];
            name = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES)[0] stringByAppendingPathComponent:name];
        }
        else {
            name = [name stringByReplacingOccurrencesOfString:@"file://" withString:@""];
        }
        return [@"file://" stringByAppendingString:name];
    }
    else {
        return name;
    }
}

+ (NSString*)md5WithString:(NSString*)text {
    if ([text length] <= 0) {
        return nil;
    }
    const char* cString = [text UTF8String];
    unsigned char result[32];
    CC_MD5(cString, (CC_LONG)strlen(cString), result);
    return [NSString stringWithFormat:@"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X", result[0], result[1], result[2], result[3], result[4], result[5], result[6], result[7], result[8], result[9], result[10], result[11], result[12], result[13], result[14], result[15]];
}

+ (NSString*)cachedFileWithURL:(NSString*)name {
    NSString* url = [AEURLCache absoluteString:name];
    if ([url hasPrefix:@"file://"]) {
        return [url stringByReplacingOccurrencesOfString:@"file://" withString:@""];
    }
    NSString* md5  = [AEURLCache md5WithString:url];
    NSString* path = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES)[0];
    NSString* file = [path stringByAppendingPathComponent:md5];
    if (![NSFileManager.defaultManager fileExistsAtPath:file]) {
        NSData* data = [NSData dataWithContentsOfURL:[NSURL URLWithString:url]];
        [data writeToFile:file atomically:YES];
    }
    return file;
}

@end
