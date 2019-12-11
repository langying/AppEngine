//
//  AEPool.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/4/19.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include "AEPool.hpp"

AEPool::~AEPool()  {
    std::unique_lock<std::mutex> lock(mutex);
    stop = true;
    condition.notify_all();
}
AEPool::AEPool(int size) {
    stop = false;
    for (int i = 0; i < size; i++) {
        threads.emplace_back(std::bind(&AEPool::run, this));
        threads[i].detach();
    }
}
void AEPool::run() {
    while (true) {
        AETask task;
        {
            std::unique_lock<std::mutex> lock(mutex);
            if (stop) {
                break;
            }
            if (tasks.empty()) {
                condition.wait(lock);
                continue;
            }
            else {
                task = std::move(tasks.front());
                tasks.pop();
            }
        }
        task();
    }
}
void AEPool::add(AETask task)  {
    std::unique_lock<std::mutex> lock(mutex);
    tasks.emplace(task);
    condition.notify_one();
}
