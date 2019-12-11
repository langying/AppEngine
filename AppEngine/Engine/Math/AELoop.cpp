//
//  AELoop.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/4/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AELoop.hpp"

AELoop::~AELoop() {
    pthread_mutex_lock(&mutex);
    stop = true;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_join(thid, nullptr);
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}

AELoop::AELoop() {
    stop = false;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 2 * 1024 * 1024);
    pthread_create(&thid, &attr /* nullable */, AELoop::run, this);
    pthread_cond_init(&cond, nullptr);
    pthread_mutex_init(&mutex, nullptr);
}

void AELoop::add(AEItem *item) {
    pthread_mutex_lock(&mutex);
    queue.push(item);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void* AELoop::run(void *self) {
    AELoop* thiz = static_cast<AELoop*>(self);
    while (true) {
        if (thiz->stop){
            break;
        }
        pthread_mutex_lock(&thiz->mutex);
        if (thiz->queue.empty()){
            pthread_cond_wait(&thiz->cond, &thiz->mutex);
            pthread_mutex_unlock(&thiz->mutex);
        }
        else {
            AEItem* item = thiz->queue.front();
            thiz->queue.pop();
            pthread_mutex_unlock(&thiz->mutex);
            
            item->run(thiz);
            delete item;
        }
    }
    return self;
}
