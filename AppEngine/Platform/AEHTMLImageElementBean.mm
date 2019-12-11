//
//  AEHTMLImageElementBean.mm
//  AppEngine
//
//  Created by 韩琼 on 2016/9/28.
//  Copyright © 2016年 Amaze. All rights reserved.
//


#import "AEFormat.hpp"
#import "AEManager.hpp"
#import "AEHTMLImageElement.hpp"
#import "AEHTMLImageElementBean.hpp"

#import "AEURLCache.h"
#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>

std::map<AEHTMLImageElementBean*, int> AEHTMLImageElementBean::beans;
void AEHTMLImageElementBean::response(AEHTMLImageElementBean* bean, std::string src, GLuint width, GLuint height) {
    auto it = beans.find(bean);
    if (it == beans.end()) {
        return;
    }
    if (bean->src != src) {
        return;
    }
    beans.erase(bean);
    bean->img->onFinish(width, height);
}

AEHTMLImageElementBean::~AEHTMLImageElementBean() {
    auto it = beans.find(this);
    if (it != beans.end()) {
        beans.erase(it);
    }
}
AEHTMLImageElementBean::AEHTMLImageElementBean(AEHTMLImageElement* image) {
    img = image;
    mgr = nullptr;
}
void AEHTMLImageElementBean::init(AEManager *mgr) {
    this->mgr = mgr;
}
void AEHTMLImageElementBean::setSrc(const char* src_) {
    beans[this] = 0;
    this->src = mgr->absURL(src_);
    
    NSString*  url = [NSString stringWithUTF8String:src.c_str()];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSData*   data = nil;
        NSString* path = [AEURLCache cachedFileWithURL:url];
        
        NSRange range = [path rangeOfString:@"!"];
        if (range.location == NSNotFound) {
            data = [NSData dataWithContentsOfFile:path];
        }
        else {
            NSString*  file = [path substringToIndex:range.location];
            NSString*  args = [path substringFromIndex:range.location + 1];
            NSArray*   list = [args componentsSeparatedByString:@"x"];
            NSInteger  seek = [list[0] integerValue];
            NSInteger  size = [list[1] integerValue];
            NSMutableData* buff = [NSMutableData dataWithLength:size];
            FILE* fp = fopen(file.UTF8String, "rb");
            fseek(fp, seek, SEEK_SET);
            fread(buff.mutableBytes, 1, size, fp);
            fclose(fp);
            data = buff;
        }
        UIImage*  image  = [UIImage imageWithData:data];
        GLuint    width  = static_cast<GLuint>(image.size.width);
        GLuint    height = static_cast<GLuint>(image.size.height);
        dispatch_async(dispatch_get_main_queue(), ^{
            AEHTMLImageElementBean::response(this, url.UTF8String, width, height);
        });
    });
}
void AEHTMLImageElementBean::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
    NSString* url  = [NSString stringWithUTF8String:src.c_str()];
    NSString* path = [AEURLCache cachedFileWithURL:url];
    
    NSData* data = nil;
    NSRange range = [path rangeOfString:@"!"];
    if (range.location == NSNotFound) {
        data = [NSData dataWithContentsOfFile:path];
    }
    else {
        NSString*  file = [path substringToIndex:range.location];
        NSString*  args = [path substringFromIndex:range.location + 1];
        NSArray*   list = [args componentsSeparatedByString:@"x"];
        NSInteger  seek = [list[0] integerValue];
        NSInteger  size = [list[1] integerValue];
        NSMutableData* buff = [NSMutableData dataWithLength:size];
        FILE* fp = fopen(file.UTF8String, "rb");
        fseek(fp, seek, SEEK_SET);
        fread(buff.mutableBytes, 1, size, fp);
        fclose(fp);
        data = buff;
    }
    CGImageRef  cgimg  = [UIImage imageWithData:data].CGImage;
    size_t      width  = CGImageGetWidth(cgimg);
    size_t      height = CGImageGetHeight(cgimg);
    size_t      btsize = CGImageGetBitsPerComponent(cgimg);
    GLvoid*     buffer = calloc(width * height, sizeof(GLuint));
    
    CGColorSpaceRef color = CGColorSpaceCreateDeviceRGB();
    CGContextRef    cgctx = CGBitmapContextCreate(buffer, width, height, btsize, width * 4, color, kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(cgctx, CGRectMake(0, 0, width, height), cgimg);
    CGContextRelease(cgctx);
    CGColorSpaceRelease(color);
    
    GLuint w = static_cast<GLuint>(width);
    GLuint h = static_cast<GLuint>(height);
    AEFormat::convert(buffer, w, h, flipY, alpha); // 这里可能会改变bitmap的底层pixels
    glTexImage2D(target, level, infmt, w, h, 0, format, type, buffer);
    free(buffer);
}
void AEHTMLImageElementBean::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
    NSString* url  = [NSString stringWithUTF8String:src.c_str()];
    NSString* path = [AEURLCache cachedFileWithURL:url];
    
    NSData* data = nil;
    NSRange range = [path rangeOfString:@"!"];
    if (range.location == NSNotFound) {
        data = [NSData dataWithContentsOfFile:path];
    }
    else {
        NSString*  file = [path substringToIndex:range.location];
        NSString*  args = [path substringFromIndex:range.location + 1];
        NSArray*   list = [args componentsSeparatedByString:@"x"];
        NSInteger  seek = [list[0] integerValue];
        NSInteger  size = [list[1] integerValue];
        NSMutableData* buff = [NSMutableData dataWithLength:size];
        FILE* fp = fopen(file.UTF8String, "rb");
        fseek(fp, seek, SEEK_SET);
        fread(buff.mutableBytes, 1, size, fp);
        fclose(fp);
        data = buff;
    }
    CGImageRef  cgimg  = [UIImage imageWithData:data].CGImage;
    size_t      width  = CGImageGetWidth(cgimg);
    size_t      height = CGImageGetHeight(cgimg);
    size_t      btsize = CGImageGetBitsPerComponent(cgimg);
    GLvoid*     buffer = calloc(width * height, sizeof(GLuint));
    
    CGColorSpaceRef color = CGColorSpaceCreateDeviceRGB();
    CGContextRef    cgctx = CGBitmapContextCreate(buffer, width, height, btsize, width * 4, color, kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(cgctx, CGRectMake(0, 0, width, height), cgimg);
    CGContextRelease(cgctx);
    CGColorSpaceRelease(color);
    
    GLuint w = static_cast<GLuint>(width);
    GLuint h = static_cast<GLuint>(height);
    AEFormat::convert(buffer, w, h, flipY, alpha); // 这里可能会改变bitmap的底层pixels
    glTexSubImage2D(target, level, xoffset, yoffset, w, h, format, type, buffer);
    free(buffer);
}
