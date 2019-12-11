//
//  AEWebSocketBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEWebSocketBean_hpp
#define AEWebSocketBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEWebSocket;
class AEWebSocketBean {
public:
	~AEWebSocketBean();
	AEWebSocketBean(AEWebSocket* bean);
	void init(AEManager* mgr, const char* url, const char* protocols);
	void send(const char* text);
	void send(const void* data, int size);
	void close(uint code, const char* reason);

	void onopen(int status, const char* message);
	void onclose(int code, const char* reason, bool remote);
	void onerror(const char* message);
	void onmessage(const char* message);

public:
	jobject impl;
	AEManager* manager;
	AEWebSocket* webSocket;
	enum {
		CONNECTING = 0, OPEN, CLOSING, CLOSED
	};
	enum {
		CLOSE_NORMAL = 1000,	//
		CLOSE_GOING_AWAY,		//
		CLOSE_PROTOCOL_ERROR,	//
		CLOSE_UNSUPPORTED,		//
		CLOSE_RESERVED,			//
		CLOSE_NO_STATUS,		//
		CLOSE_ABNORMAL,			//
		CLOSE_UNSUPPORTED_DATA,	//
		CLOSE_POLICY_VIOLATION, //
		CLOSE_TOO_LARGE,		//
		CLOSE_MISSING_EXTENSION,//
		CLOSE_INTERNAL_ERROR,	//
		CLOSE_SERVICE_RESTART,	//
		CLOSE_TRY_AGAIN_LATER	//
	};
};

#endif /* AEWebSocketBean_hpp */
