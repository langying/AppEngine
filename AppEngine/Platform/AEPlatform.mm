//
//  AEPlatform.cpp
//  Action
//
//  Created by 韩琼 on 15/12/28.
//  Copyright © 2015年 AppEngine. All rights reserved.
//

#import "AEFormat.hpp"
#import "AEManager.hpp"
#import "AEPlatform.hpp"
#import "AEHTMLImageElement.hpp"
#import "AECanvasRenderingContext2D.hpp"

#import "AEURLCache.h"
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>

GLuint AEPlatform::ID = 0;
GLuint AEPlatform::UUID() {
    return ID++;
}

GLvoid AEPlatform::alert(AEManager* mgr, const char* text_) {
    NSString* text = [NSString stringWithUTF8String:text_];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        [[[UIAlertView.alloc initWithTitle:nil message:text delegate:nullptr cancelButtonTitle:@"好" otherButtonTitles:nil] autorelease] show];
    });
}
GLvoid AEPlatform::speak(AEManager* mgr, const char* text_, const char* lang_, float rate) {
    NSString* lang = [NSString stringWithUTF8String:lang_];
    NSString* text = [NSString stringWithUTF8String:text_];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        AVSpeechUtterance* utterance = [AVSpeechUtterance speechUtteranceWithString:text];
        utterance.rate  = rate * AVSpeechUtteranceDefaultSpeechRate;
        utterance.voice = [AVSpeechSynthesisVoice voiceWithLanguage:lang];
        [[[AVSpeechSynthesizer.alloc init] autorelease] speakUtterance:utterance];
    });
}
GLvoid AEPlatform::delItem(AEManager* mgr, const char* key_) {
    if (key_) {
        NSString* key = [NSString stringWithFormat:@"_duck_%s", key_];
        [NSUserDefaults.standardUserDefaults removeObjectForKey:key];
        [NSUserDefaults.standardUserDefaults synchronize];
    }
}
GLchar* AEPlatform::getItem(AEManager* mgr, const char* key_) {
    if (!key_) {
        return nullptr;
    }
    NSString* key = [NSString stringWithFormat:@"_duck_%s", key_];
    NSString* val = [NSUserDefaults.standardUserDefaults stringForKey:key];
    if (val.length <= 0) {
        return nullptr;
    }
    
    size_t length = strlen(val.UTF8String);
    char*  buffer = (char*)calloc(length + 1, sizeof(char));
    memcpy(buffer, val.UTF8String, length);
    return buffer;
}
GLvoid AEPlatform::setItem(AEManager* mgr, const char* key_, const char* val_) {
    if (key_) {
        NSString* key = [NSString stringWithFormat:@"_duck_%s", key_];
        NSString* val = [NSString stringWithUTF8String:val_];
        [NSUserDefaults.standardUserDefaults setObject:val forKey:key];
        [NSUserDefaults.standardUserDefaults synchronize];
    }
}
GLchar* AEPlatform::textWithURL(AEManager* mgr, const char* url) {
    NSString* txt = [AEURLCache textWithURL:[NSString stringWithUTF8String:url]];
    txt = txt ?: @"";
    size_t len = strlen(txt.UTF8String);
    void*  buf = calloc(len + 1, sizeof(GLchar));
    memcpy(buf, txt.UTF8String, len);
    return static_cast<GLchar*>(buf);
}

std::string AEPlatform::encodeURL(const char* url_) {
    NSString* url = [NSString stringWithUTF8String:url_];
    if (url.length <= 0) {
        return "";
    }
    CFStringRef pattern = (CFStringRef)@"!*'();:@&=+$,/?%#[]";
    NSString* ret = (NSString*)CFBridgingRelease(CFURLCreateStringByAddingPercentEscapes(kCFAllocatorDefault, (CFStringRef)url, NULL, pattern, kCFStringEncodingUTF8));
    return ret.UTF8String;
}

std::string AEPlatform::decodeURL(const char* url_) {
    NSString* url = [NSString stringWithUTF8String:url_];
    if (url.length <= 0) {
        return "";
    }
    NSMutableString* src = [NSMutableString stringWithString:url];
    [src replaceOccurrencesOfString:@"+" withString:@" " options:NSLiteralSearch range:NSMakeRange(0, src.length)];
    NSString* ret = [src stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
    return ret.UTF8String;
}

//void AEPlatform::fileToUTF8String(std::string url, std::function<void(int, const char*)> handle) {
//    NSString* file = [AEURLCache absoluteString:[NSString stringWithUTF8String:url.c_str()]];
//    NSData*   data = [NSData dataWithContentsOfURL:[NSURL URLWithString:file]];
//    if (data) {
//        NSString* rets = [NSString.alloc initWithData:data encoding:CFStringConvertEncodingToNSStringEncoding(kCFStringEncodingGB_18030_2000)];
//        const char* data = rets.UTF8String;
//        handle(static_cast<int>(strlen(data)), data);
//    }
//    else {
//        handle(0, NULL);
//    }
//}
