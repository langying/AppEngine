//
//  AEWebSocket.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEManager.hpp"
#include "AEWebSocket.hpp"

AEWebSocket::~AEWebSocket() {
	bean->close(0, "release close.");
	delete bean;
}
AEWebSocket::AEWebSocket() {
	cx = nullptr;
	ID = AEPlatform::UUID();
	bean = new AEWebSocketBean(this);

	readyState = AEWebSocketBean::CLOSED;
	bufferedAmount = 0;
}
void AEWebSocket::init(duk_context* ctx, const char* url, const char* protocol) {
	duk_push_global_stash(ctx);		// ... stash
	duk_push_this(ctx);				// ... stash this
	duk_put_prop_index(ctx, -2, ID);// ... stash[ID] = this
	duk_pop(ctx);					// ...
	bean->init(AEManager::find(ctx), url, protocol);
}
void AEWebSocket::onopen(const char* message, int status) {
	onCallback("onopen", message, status, false);
}
void AEWebSocket::onclose(const char* message, int code, bool remote) {
	onCallback("onclose", message, code, remote);
}
void AEWebSocket::onerror(const char* message) {
	onCallback("onerror", message, 0, false);
}
void AEWebSocket::onmessage(const char* message) {
	onCallback("onmessage", message, 0, false);
}
void AEWebSocket::onCallback(const char* type, const char* message, int code, bool remote) {
    duk_push_global_stash(cx);      // ... stash
    duk_get_prop_index(cx, -1, ID);	// ... stash this
    duk_push_string(cx, "onFinish");// ... stash this onFinish
    duk_push_string(cx, type);		// ... stash this onFinish type
    duk_push_string(cx, message);	// ... stash this onFinish type message
    duk_push_uint(cx, code);		// ... stash this onFinish type message code
    duk_push_boolean(cx, remote);	// ... stash this onFinish type message code remote
    duk_call_prop(cx, -6, 4);       // ... stash this result
    duk_pop_3(cx);                  // ...
}

int AEWebSocket::send(duk_context* ctx) {
	js_thiz(AEWebSocket);
	if (duk_is_string(ctx, 0)) {
		const char* val = duk_get_string(ctx, 0);
		thiz->bean->send(val);
	}
	else {
		size_t  size = 0;
		GLvoid* data = duk_get_buffer_data(ctx, 0, &size);
		thiz->bean->send(data, static_cast<int>(size));
	}
	return 0;
}
int AEWebSocket::close(duk_context* ctx) {
	js_thiz(AEWebSocket);
	thiz->bean->close(0, "namorl close.");
	return 0;
}

void AEWebSocket::binding(duk_context *ctx) {
	js_begin_binding();
	js_add_prop(readyState);
	js_add_prop(bufferedAmount);
	js_add_prop(url);
	js_add_prop(protocol);
	js_add_prop(binaryType);
	js_add_prop(extensions);

	js_add_value(int, 0, CONNECTING);
	js_add_value(int, 1, OPEN);
	js_add_value(int, 2, CLOSING);
	js_add_value(int, 3, CLOSED);

	js_binding_func( 1, send);
	js_binding_func(-1, close);
	js_end_binding(WebSocket);
}
