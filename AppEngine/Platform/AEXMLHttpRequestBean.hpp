//
//  AEXMLHttpRequestBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/1.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEXMLHttpRequestBean_hpp
#define AEXMLHttpRequestBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEXMLHttpRequest;
class AEXMLHttpRequestBean {
public:
	jobject	impl;
	GLvoid*	data;
	GLuint	size;
	GLuint	status;
	GLuint	timeout;
	GLuint	readyState;
	std::string method;
	std::string mimeType;
	std::string statusText;
	std::string responseURL;
	std::string responseType;
	AEManager*		  manager;
	AEXMLHttpRequest* request;
	std::map<std::string, std::string> reqHeaders;
	std::map<std::string, std::string> retHeaders;

public:
	~AEXMLHttpRequestBean();
	AEXMLHttpRequestBean(AEXMLHttpRequest* request);

	void init(AEManager* mgr);
	void open(const char* method, const char* url, const bool async, const char* username, const char* password);
	void send();
	void abort();
	void reset(bool async, const char* url, const char* method);
	void overrideMimeType(const char* mimetype);
	void setRequestHeader(const char* header, const char* value);
	void onFinish(int size, const void* data, const char* mime, const char* type);

	std::string getResponseHeader(const char* header);
	std::string getAllResponseHeaders();

public:
	uint getStatus();
	uint getTimeout();
	uint getReadyState();
	std::string getMimeType();
	std::string getStatusText();
	std::string getResponseURL();
	std::string getResponseType(); void setResponseType(const char* val);
};

#endif /* AEXMLHttpRequestBean_hpp */
