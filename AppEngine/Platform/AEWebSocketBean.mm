//
//  AEWebSocketBean.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/4.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEWebSocketBean.hpp"

AEWebSocketBean::~AEWebSocketBean() {
    close(0, "destructor");
}
AEWebSocketBean::AEWebSocketBean(AEWebSocket* socket) {
    this->impl = nullptr;
    this->manager = nullptr;
    this->webSocket = socket;
}

void AEWebSocketBean::init(AEManager* mgr, const char* url_, const char* ptl_) {
    this->manager = mgr;
}

void AEWebSocketBean::send(const char* text_) {
}
void AEWebSocketBean::send(const void* data_, int size) {
}
void AEWebSocketBean::close(uint code, const char* reason_) {
}

void AEWebSocketBean::onopen(int status, const char* message) {
}
void AEWebSocketBean::onclose(int code, const char* reason, bool remote) {
}
void AEWebSocketBean::onerror(const char* message) {
}
void AEWebSocketBean::onmessage(const char* message) {
}
