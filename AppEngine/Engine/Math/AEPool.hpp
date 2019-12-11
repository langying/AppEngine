//
//  AEPool.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/4/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEPool_hpp
#define AEPool_hpp

#include <queue>
#include <thread>
#include "AEPlatform.hpp"

typedef std::function<void()> AETask;

class AEPool {
protected:
    boolean            stop;
    std::queue<AETask> tasks;
    std::mutex               mutex;
    std::condition_variable  condition;
    std::vector<std::thread> threads;
    
public:
    ~AEPool();
    AEPool(int size = 1);
    void run();
    void add(AETask task);
};

#endif /* AEPool_hpp */
