//
//  AEWebSocket.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEWebSocket_hpp
#define AEWebSocket_hpp

#include "AEWebSocketBean.hpp"

class AEWebSocket {
public:
	js_func(send);
	js_func(close);

	js_prop(AEWebSocket, uint,   readyState);
	js_prop(AEWebSocket, uint,   bufferedAmount);
	js_prop(AEWebSocket, string, url);
	js_prop(AEWebSocket, string, protocol);
	js_prop(AEWebSocket, string, binaryType);
	js_prop(AEWebSocket, string, extensions);

	js_con_destructor(AEWebSocket, [](duk_context* ctx, AEWebSocket* thiz, GLint argc) {
        const char* url = argc <= 0 ? nullptr : duk_get_string(ctx, 0);
        const char* pro = argc <= 1 ? nullptr : duk_get_string(ctx, 1);
        thiz->init(ctx, url, pro);
	});

public:
	GLuint ID;
	AEWebSocketBean* bean;
	void init(duk_context* ctx, const char* url, const char* protocol);
	void onopen(const char* message, int status);
	void onclose(const char* message, int code, bool remote);
	void onerror(const char* message);
	void onmessage(const char* message);
	void onCallback(const char* type, const char* message, int code, bool remote);
};

#endif /* AEWebSocket_hpp */
