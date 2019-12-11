//
//  AEXMLHttpRequest.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/16.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEXMLHttpRequest_hpp
#define AEXMLHttpRequest_hpp

#include "AEXMLHttpRequestBean.hpp"

class AEXMLHttpRequest {
public:
	js_define(status);
	js_define(statusText);
	js_define(readyState);
	js_define(timeout);
	js_define(mimeType);
    js_define(response);
    js_define(responseURL);
    js_define(responseText);
	js_define(responseType);
    js_con_destructor(AEXMLHttpRequest, [](duk_context* ctx, AEXMLHttpRequest* thiz, GLint argc) {
    	thiz->init(ctx);
    });
    
public:
    js_func(open);
    js_func(send);
    js_func(abort);
    js_func(overrideMimeType);
    js_func(setRequestHeader);
    js_func(getResponseHeader);
    js_func(getAllResponseHeaders);

public:
    GLuint ID;
    AEXMLHttpRequestBean* bean;
    void init(duk_context* ctx);
    void onFinish();
};

#endif /* AEXMLHttpRequest_hpp */
