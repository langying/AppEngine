//
//  AECanvasRenderingContext2DBean.mm
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <CoreGraphics/CoreGraphics.h>

#import "AEFormat.hpp"
#import "AEURLCache.h"
#import "AECanvasRenderingContext2DBean.hpp"

AECanvasRenderingContext2DBean::~AECanvasRenderingContext2DBean() {
    ae_free(impl);
    LOG("delete bitmap[%dx%d][%d KB].", _width, _height, _width * _height * 4 / 1024);
}
AECanvasRenderingContext2DBean::AECanvasRenderingContext2DBean() {
    impl       = nullptr;
    _width     = 300;
    _height    = 150;
    _fillColor = 0;
    _fontColor = 0;
    _fontSize  = 16;
    _lineWidth = 1;
    _fontName  = "";
    
    resize(_width, _height);
}
void AECanvasRenderingContext2DBean::setWidth(GLuint val) {
    _width = val;
}
void AECanvasRenderingContext2DBean::setHeight(GLuint val) {
    _height = val;
}
void AECanvasRenderingContext2DBean::setFillColor(GLuint val) {
    _fillColor = val;
}
void AECanvasRenderingContext2DBean::setFontColor(GLuint val) {
    _fontColor = val;
}
void AECanvasRenderingContext2DBean::setFontSize(GLfloat val) {
    _fontSize = val;
}
void AECanvasRenderingContext2DBean::setLineWidth(GLfloat val) {
    _lineWidth = val;
}
void AECanvasRenderingContext2DBean::setFontName(GLstring val) {
    _fontName = val;
}
void AECanvasRenderingContext2DBean::setFillStyle(GLstring val) {
    _fillStyle = val;
}

void AECanvasRenderingContext2DBean::init(AEManager* mgr) {
}
void AECanvasRenderingContext2DBean::resize(int width, int height) {
    ae_free(impl);
    _width  = width;
    _height = height;
    if (width && height) {
        impl = calloc(width * height, sizeof(GLuint));
    }
}
void AECanvasRenderingContext2DBean::texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool flipY, bool alpha) {
	AEFormat::convert(impl, _width, _height, flipY, alpha); // 这里可能会改变bitmap的底层pixels
    glTexImage2D(target, level, infmt, _width, _height, 0, format, type, impl);
}
void AECanvasRenderingContext2DBean::texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool flipY, bool alpha) {
	AEFormat::convert(impl, _width, _height, flipY, alpha); // 这里可能会改变bitmap的底层pixels
    glTexSubImage2D(target, level, xoffset, yoffset, _width, _height, format, type, impl);
}

void AECanvasRenderingContext2DBean::clearRect(int x, int y, int w, int h) {
    CGColorSpaceRef color = CGColorSpaceCreateDeviceRGB();
    CGContextRef    cgctx = CGBitmapContextCreate(impl, _width, _height, 8, _width * 4, color, kCGImageAlphaPremultipliedLast);
    
    CGContextClearRect(cgctx, CGRectMake(x, y, w, h));
    
    CGContextRelease(cgctx);
    CGColorSpaceRelease(color);
}
void AECanvasRenderingContext2DBean::drawImage(AEImage* img, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh) {
    CGColorSpaceRef color = CGColorSpaceCreateDeviceRGB();
    CGContextRef    cgctx = CGBitmapContextCreate(impl, _width, _height, 8, _width * 4, color, kCGImageAlphaPremultipliedLast);
    
    NSString* src = [NSString stringWithUTF8String:img->pathfile().c_str()];
    NSString* url = [AEURLCache cachedFileWithURL:src];
    CGContextDrawImage(cgctx, CGRectMake(dx, dy, dw, dh), [UIImage imageWithContentsOfFile:url].CGImage);
    
    CGContextRelease(cgctx);
    CGColorSpaceRelease(color);
}
void AECanvasRenderingContext2DBean::fillRect(int x, int y, int w, int h) {
    CGColorSpaceRef color = CGColorSpaceCreateDeviceRGB();
    CGContextRef    cgctx = CGBitmapContextCreate(impl, _width, _height, 8, _width * 4, color, kCGImageAlphaPremultipliedLast);
    
    CGFloat  r = ((_fillColor >> 16) & 0xFF) / 255.0;
    CGFloat  g = ((_fillColor >>  8) & 0xFF) / 255.0;
    CGFloat  b = ((_fillColor >>  0) & 0xFF) / 255.0;
    UIColor* c = [UIColor colorWithRed:r green:g blue:b alpha:1.0];
    CGContextSetLineWidth(cgctx, _lineWidth);
    CGContextSetFillColorWithColor(cgctx, c.CGColor);
    CGContextFillRect(cgctx, CGRectMake(x, y, w, h));
    
    CGContextRelease(cgctx);
    CGColorSpaceRelease(color);
}
void AECanvasRenderingContext2DBean::fillText(const char* text, int x, int y) {
    CGColorSpaceRef color = CGColorSpaceCreateDeviceRGB();
    CGContextRef    cgctx = CGBitmapContextCreate(impl, _width, _height, 8, _width * 4, color, kCGImageAlphaPremultipliedLast);
    
    CGFloat r = ((_fillColor >> 16) & 0xFF) / 255.0;
    CGFloat g = ((_fillColor >>  8) & 0xFF) / 255.0;
    CGFloat b = ((_fillColor >>  0) & 0xFF) / 255.0;
    CGPoint       point = CGPointMake(x, y - _fontSize);
    NSString*     words = [NSString stringWithUTF8String:text];
    NSDictionary* attrs = @{NSFontAttributeName:[UIFont systemFontOfSize:_fontSize], NSForegroundColorAttributeName:[UIColor colorWithRed:r green:g blue:b alpha:1.0]};
    
    UIGraphicsPushContext(cgctx);
    CGContextSaveGState(cgctx);
    CGContextTranslateCTM(cgctx, 0, _height);
    CGContextScaleCTM(cgctx, 1.0, -1.0);
    CGContextSetLineWidth(cgctx, _lineWidth);
    [words drawAtPoint:point withAttributes:attrs];
    CGContextRestoreGState(cgctx);
    UIGraphicsPopContext();
    
    CGContextRelease(cgctx);
    CGColorSpaceRelease(color);
}
void AECanvasRenderingContext2DBean::getImageData(int x, int y, int w, int h, void *pixels) {
    memcpy(pixels, impl, _width * _height * sizeof(GLuint));
}
float AECanvasRenderingContext2DBean::measureText(const char* text_) {
    NSString*     text = [NSString stringWithUTF8String:text_];
    NSDictionary* attr = @{NSFontAttributeName:[UIFont systemFontOfSize:_fontSize]};
    CGSize        size = [text sizeWithAttributes:attr];
    return size.width;
}
void AECanvasRenderingContext2DBean::scale(float x, float y) {
}
void AECanvasRenderingContext2DBean::strokeText(const char* text, int x, int y) {
    // CGContextSetStrokeColorWithColor(ctx, c.CGColor);
    fillText(text, x, y);
}
