//
//  AEXMLHttpRequestBean.mm
//  AppEngine
//
//  Created by 韩琼 on 2016/12/1.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEManager.hpp"
#include "AEXMLHttpRequest.hpp"
#include "AEXMLHttpRequestBean.hpp"

#import "AEURLCache.h"
#import <Foundation/Foundation.h>

std::map<AEXMLHttpRequestBean*, int> beans;
void callback(AEXMLHttpRequestBean* bean, NSString* url, NSString* mime, NSString* type, NSData* data, NSURLResponse* response) {
    auto it = beans.find(bean);
    if (it == beans.end()) {
        return;
    }
    beans.erase(bean);
    if ([response isKindOfClass:NSHTTPURLResponse.class]) {
        NSDictionary* fields = [(NSHTTPURLResponse*)response allHeaderFields];
        [fields enumerateKeysAndObjectsUsingBlock:^(NSString* key, NSString* val, BOOL* stop) {
            bean->retHeaders[key.UTF8String] = val.UTF8String ?: "";
        }];
    }
    bean->onFinish((uint)data.length, data.bytes, mime.UTF8String, type.UTF8String);
}

AEXMLHttpRequestBean::~AEXMLHttpRequestBean() {
    abort();
}
AEXMLHttpRequestBean::AEXMLHttpRequestBean(AEXMLHttpRequest* request) {
    this->impl = nullptr;
    this->data = nullptr;
    this->manager = nullptr;
    this->request = request;
    reset(true, nullptr, nullptr);
}
void AEXMLHttpRequestBean::init(AEManager* mgr) {
    this->manager = mgr;
}
void AEXMLHttpRequestBean::open(const char* method_, const char* url_, const bool async, const char* username_, const char* password_) {
    reset(async, url_, method_);
}
void AEXMLHttpRequestBean::send() {
    beans[this] = 0;
    auto thiz    = this;
    auto timeout = this->timeout / 1000.0;
    NSString* url    = [NSString stringWithUTF8String:responseURL.c_str()];
    NSString* method = [NSString stringWithUTF8String:this->method.c_str()];
    NSMutableDictionary* params = [NSMutableDictionary dictionary];
    for (auto it = reqHeaders.begin(); it != reqHeaders.end(); it++) {
        NSString* key = [NSString stringWithUTF8String:it->first.c_str()];
        NSString* val = [NSString stringWithUTF8String:it->second.c_str()];
        params[key] = val;
    }
    NSString* mime_ = [NSString stringWithUTF8String:this->mimeType.c_str()];
    NSString* type_ = [NSString stringWithUTF8String:this->responseType.c_str()];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
        NSError* error = nil;
        NSURLResponse* response = nil;
        NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[AEURLCache absoluteString:url]]];
        request.HTTPMethod      = method;
        request.timeoutInterval = timeout;
        [params enumerateKeysAndObjectsUsingBlock:^(NSString* key, NSString* val, BOOL* stop) {
            [request setValue:val forHTTPHeaderField:key];
        }];
        NSData* data = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
        if (error) {
            LOG("[%s]:%s", url.UTF8String, error.domain.UTF8String);
        }
        dispatch_async(dispatch_get_main_queue(), ^{
            NSString* mime = mime_;
            NSString* type = type_;
            if (mime.length <= 0) {
                mime = response.MIMEType;
            }
            if (mime.length <= 0) {
                mime = @"text/plain";
            }
            if (type.length <= 0) {
                if ([mime containsString:@"text"] || [mime containsString:@"json"] || [mime containsString:@"javascript"]) {
                    type = @"text";
                }
                else {
                    type = @"blob";
                }
            }
            callback(thiz, url, mime, type, data, response);
        });
    });
}
void AEXMLHttpRequestBean::abort() {
    auto it = beans.find(this);
    if (it != beans.end()) {
        beans.erase(it);
    }
    reset(true, nullptr, nullptr);
}
void AEXMLHttpRequestBean::reset(bool async, const char* url, const char* method) {
    if (this->data) {
        free(this->data);
    }
    this->data			= nullptr;
    this->size			= 0;
    this->status		= 0;
    this->timeout		= 3000;
    this->readyState	= 0;
    this->method		= method ? : "GET";
    this->mimeType		= "";
    this->statusText	= "";
    this->responseURL	= url ? : "";
    this->responseType	= "";
    this->reqHeaders.clear();
    this->retHeaders.clear();
}
void AEXMLHttpRequestBean::overrideMimeType(const char* mimetype_) {
    mimeType = mimetype_;
}
void AEXMLHttpRequestBean::setRequestHeader(const char* key, const char* val) {
    reqHeaders[key] = val;
}
void AEXMLHttpRequestBean::onFinish(int size, const void* data, const char* mime, const char* type) {
    this->size = size;
    this->data = malloc(size);
    this->status       = 200;
    this->mimeType     = mime;
    this->responseType = type;
    memcpy(this->data, data, size);
    this->request->onFinish();
}

std::string AEXMLHttpRequestBean::getResponseHeader(const char* key) {
    auto it = retHeaders.find(key);
    if (it == retHeaders.end()) {
        return "";
    }
    else {
        return it->second;
    }
}
std::string AEXMLHttpRequestBean::getAllResponseHeaders() {
    return "";
}

uint AEXMLHttpRequestBean::getStatus() {
    return status;
}
uint AEXMLHttpRequestBean::getTimeout() {
    return timeout;
}
uint AEXMLHttpRequestBean::getReadyState() {
    return readyState;
}
std::string AEXMLHttpRequestBean::getMimeType() {
    return mimeType;
}
std::string AEXMLHttpRequestBean::getStatusText() {
    return statusText;
}
std::string AEXMLHttpRequestBean::getResponseURL() {
    return responseURL;
}
std::string AEXMLHttpRequestBean::getResponseType() {
    return responseType;
}
void AEXMLHttpRequestBean::setResponseType(const char* type_) {
    responseType = type_;
}
