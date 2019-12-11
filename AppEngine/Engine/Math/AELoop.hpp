//
//  AELoop.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/4/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AELoop_hpp
#define AELoop_hpp

#include <queue>
#include <pthread.h>
#include "AEPlatform.hpp"

class AELoop;
class AEItem {
public:
    AEItem();
    virtual ~AEItem() {};
    virtual void run(AELoop* loop) = 0;
};

class AELoop {
private:
    boolean             stop;
    pthread_t           thid;
    pthread_cond_t      cond;
    pthread_mutex_t     mutex;
    std::queue<AEItem*> queue;
    
public:
    ~AELoop();
    AELoop();
    void add(AEItem* item);
    
public:
    static void* run(void* self);
};

#endif /* AELoop_hpp */
