//
//  AETimer.hpp
//  Amaze
//
//  Created by 韩琼 on 16/1/21.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AETimer_hpp
#define AETimer_hpp

#include "AEPlatform.hpp"

#define MIN_WAIT  1.0
#define MIN_DELAY 1.0
#define MAX_COUNT 4096

typedef struct {
    GLuint  ID;
    boolean oneshot;
    boolean removed;
    double  target;
    double  delay;
} AEFunc;

class AETimer {
public:
    static uint UID;
    
public:
    GLuint count;
    AEFunc expiring;
    AEFunc funcs[MAX_COUNT];
    duk_context* ctx;
    
public:
    ~AETimer();
    AETimer(duk_context* ctx);
    GLvoid resort();
    GLuint update();
    GLuint create();
    GLuint remove();
};

#endif /* AETimer_hpp */
