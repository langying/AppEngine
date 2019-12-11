//
//  AEXMLHttpRequest.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/5/16.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEManager.hpp"
#include "AEXMLHttpRequest.hpp"

AEXMLHttpRequest::~AEXMLHttpRequest() {
	bean->abort();
	delete bean;
}
AEXMLHttpRequest::AEXMLHttpRequest() {
	cx   = nullptr;
    ID   = AEPlatform::UUID();
    bean = new AEXMLHttpRequestBean(this);
}
void AEXMLHttpRequest::init(duk_context* ctx) {
	bean->init(AEManager::find(ctx));
}
void AEXMLHttpRequest::onFinish() {
    duk_push_global_stash(cx);      // ... stash
    duk_get_prop_index(cx, -1, ID);	// ... stash this
    duk_push_string(cx, "onFinish");// ... stash this onFinish
    duk_call_prop(cx, -2, 0);       // ... stash this result
    duk_pop_2(cx);                  // ... stash
    duk_del_prop_index(cx, -1, ID); // ... stash (delete stash[ID])
    duk_pop(cx);                    // ...
}

int AEXMLHttpRequest::get_status(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	uint val = thiz->bean->getStatus();
	duk_push_uint(ctx, val);
	return 1;
}
int AEXMLHttpRequest::set_status(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_statusText(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	std::string val = thiz->bean->getStatusText();
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEXMLHttpRequest::set_statusText(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_readyState(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	uint val = thiz->bean->getReadyState();
	duk_push_uint(ctx, val);
	return 1;
}
int AEXMLHttpRequest::set_readyState(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_timeout(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	uint val = thiz->bean->getTimeout();
	duk_push_uint(ctx, val);
	return 1;
}
int AEXMLHttpRequest::set_timeout(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_mimeType(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	std::string val = thiz->bean->getMimeType();
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEXMLHttpRequest::set_mimeType(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_response(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    if (thiz->bean->getResponseType() == "text") {
    	const char* data = static_cast<const char*>(thiz->bean->data);
    	std::string text(data, thiz->bean->size);
    	duk_push_string(ctx, text.c_str());
    }
    else if (thiz->bean->getResponseType() == "blob") {
    	uint  size = thiz->bean->size;
    	void* data = duk_push_fixed_buffer(ctx, size);
    	memcpy(data, thiz->bean->data, size);
    }
    else if (thiz->bean->getResponseType() == "arraybuffer") {
    	uint  size = thiz->bean->size;
    	void* data = duk_push_fixed_buffer(ctx, size);
    	memcpy(data, thiz->bean->data, size);
    }
    else {
    	uint  size = thiz->bean->size;
    	void* data = duk_push_fixed_buffer(ctx, size);
    	memcpy(data, thiz->bean->data, size);
    }
    return 1;
}
int AEXMLHttpRequest::set_response(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_responseURL(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	std::string val = thiz->bean->getResponseURL();
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEXMLHttpRequest::set_responseURL(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_responseText(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	if (thiz->bean->getResponseType() == "text") {
		const uint  size = thiz->bean->size;
		const char* data = static_cast<const char*>(thiz->bean->data);
		std::string text(data, size);
		duk_push_string(ctx, text.c_str());
	}
	else {
		duk_push_null(ctx);
	}
	return 1;
}
int AEXMLHttpRequest::set_responseText(duk_context* ctx) {
	return 0;
}
int AEXMLHttpRequest::get_responseType(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	std::string val = thiz->bean->getResponseType();
	duk_push_string(ctx, val.c_str());
	return 1;
}
int AEXMLHttpRequest::set_responseType(duk_context* ctx) {
	js_thiz(AEXMLHttpRequest);
	const char* val = duk_get_string(ctx, 0);
	thiz->bean->setResponseType(val);
	return 0;
}

int AEXMLHttpRequest::open(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    int argc = duk_get_top(ctx);
    const char* method	 = argc > 0 ? duk_get_string( ctx, 0) : "GET";
    const char* location = argc > 1 ? duk_get_string( ctx, 1) : "";
    const bool  async	 = argc > 2 ? duk_get_boolean(ctx, 2) : true;
    const char* username = argc > 3 ? duk_get_string( ctx, 3) : "";
    const char* password = argc > 4 ? duk_get_string( ctx, 4) : "";
    std::string url = AEManager::find(ctx)->absURL(location);
    thiz->bean->open(method, url.c_str(), async, username, password);
    return 0;
}
int AEXMLHttpRequest::send(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);				// ...
    duk_push_global_stash(ctx);             // ... stash
    duk_push_this(ctx);                     // ... stash this
    duk_put_prop_index(ctx, -2, thiz->ID);  // ... stash ([ID]=this)
    duk_pop(ctx);							// ...

    thiz->bean->send();
    return 0;
}
int AEXMLHttpRequest::abort(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    thiz->bean->abort();
    return 0;
}
int AEXMLHttpRequest::overrideMimeType(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    const char* val = duk_get_string(ctx, 0);
    thiz->bean->overrideMimeType(val);
    return 0;
}
int AEXMLHttpRequest::setRequestHeader(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    const char* key	= duk_get_string(ctx, 0);
    const char* val	= duk_get_string(ctx, 1);
    thiz->bean->setRequestHeader(key, val);
    return 0;
}
int AEXMLHttpRequest::getResponseHeader(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    const char* key = duk_get_string(ctx, 0);
    std::string val = thiz->bean->getResponseHeader(key);
    duk_push_string(ctx, val.c_str());
    return 1;
}
int AEXMLHttpRequest::getAllResponseHeaders(duk_context* ctx) {
    js_thiz(AEXMLHttpRequest);
    std::string val = thiz->bean->getAllResponseHeaders();
    duk_push_string(ctx, val.c_str());
    return 1;
}
void AEXMLHttpRequest::binding(duk_context* ctx) {
    js_begin_binding();
    js_add_prop(status);
    js_add_prop(statusText);
    js_add_prop(readyState);
    js_add_prop(timeout);
    js_add_prop(mimeType);
    js_add_prop(response);
    js_add_prop(responseURL);
    js_add_prop(responseText);
    js_add_prop(responseType);
    js_binding_func(-1, open);
    js_binding_func(-1, send);
    js_binding_func( 0, abort);
    js_binding_func( 1, overrideMimeType);
    js_binding_func( 2, setRequestHeader);
    js_binding_func( 1, getResponseHeader);
    js_binding_func( 0, getAllResponseHeaders);
    js_end_binding(XMLHttpRequest);
}
