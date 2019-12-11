//
//  AETimer.cpp
//  Amaze
//
//  Created by 韩琼 on 16/1/21.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AETimer.hpp"

#define kTimer "kTimer"

uint AETimer::UID = 0;

AETimer::~AETimer() {
    ctx = nullptr;
}


AETimer::AETimer(duk_context* ct) {
    ctx = ct;
    count = 0;
    memset(&expiring, 0, sizeof(AEFunc));
    memset(funcs, 0, sizeof(AEFunc) * MAX_COUNT);
}

void AETimer::resort() {
    AEFunc tp;
    AEFunc *t;
    for (int i = count - 1; i > 0; i--) {
        t = funcs + i;
        if ((t-1)->target > t->target) {
            break;
        }
        else {
            memcpy((void*)(&tp), (void*)(t-1), sizeof(AEFunc));
            memcpy((void*)(t-1), (void*)(t  ), sizeof(AEFunc));
            memcpy((void*)(t  ), (void*)(&tp), sizeof(AEFunc));
        }
    }
}

GLuint AETimer::update() {
    AEFunc *cb;
    GLuint num = 100;
    double now = get_now();
    
    duk_push_global_stash(ctx);             // stash
    duk_get_prop_string(ctx, -1, kTimer);   // stash kTimer
    
    while(num-- > 0) {
        if (count <= 0) {
            break;
        }
        
        cb = funcs + count - 1;
        if (cb->target > now) {
            break;
        }
        
        memcpy(&expiring, cb, sizeof(AEFunc));
        memset(cb, 0, sizeof(AEFunc));
        
        count--;
        cb = &expiring;
        if (cb->oneshot) {
            cb->removed = GL_TRUE;
        }
        else {
            cb->target = cb->target + cb->delay;
        }
        
        duk_get_prop_index(ctx, -1, cb->ID); // stash kTimer callback
        duk_pcall(ctx, 0);                   // stash kTimer result
        duk_pop(ctx);                        // stash kTimer
        
        if (cb->removed) {
            duk_del_prop_index(ctx, -1, cb->ID);
        }
        else {
            if (count >= MAX_COUNT) {
                LOG("finishTimer out of timer slots(%d).", MAX_COUNT);
                duk_error(ctx, DUK_ERR_RANGE_ERROR, "out of timer slots.");
            }
            memcpy(funcs + count, cb, sizeof(AEFunc));
            count++;
            resort();
        }
    }
    memset(&expiring, 0, sizeof(AEFunc));
    duk_pop_2(ctx);
    return 0;
}

GLuint AETimer::create() {
    UID++;
    double  timeout = duk_get_number(ctx, 1);
    boolean oneshot = duk_get_boolean(ctx, 2);
    if (timeout < MIN_DELAY) {
        timeout = MIN_DELAY;
    }
    if (count >= MAX_COUNT) {
        LOG("createTimer: out of timer slots(%d).", MAX_COUNT);
        duk_error(ctx, DUK_ERR_RANGE_ERROR, "out of timer slots.");
    }
    
    double now = get_now();
    GLuint idx = count++;
    AEFunc *cb = funcs + idx;
    
    memset(cb, 0, sizeof(AEFunc));
    cb->ID      = AETimer::UID;
    cb->target  = now + timeout;
    cb->delay   = timeout;
    cb->oneshot = oneshot;
    cb->removed = GL_FALSE;
    resort();
    
    
    duk_push_global_stash(ctx);                 // stash
    duk_get_prop_string(ctx, -1, kTimer);       // stash kTimer
    duk_dup(ctx, 0);                            // stash kTimer callback
    duk_put_prop_index(ctx, -2, AETimer::UID);  // sgash kTimer (stash.kTimer[UID] = callback;)
    duk_pop_2(ctx);                             // none
    duk_push_uint(ctx, AETimer::UID);           // CallbackID
    return 1;
}

GLuint AETimer::remove() {
    GLuint ID = duk_get_uint(ctx, 0);
    
    AEFunc *t, *func = &expiring;
    if (func->ID == ID) {
        func->removed = 1;
        duk_push_true(ctx);
        return 1;
    }
    
    boolean found = GL_FALSE;
    for (GLuint i = 0; i < count; i++) {
        t = funcs + i;
        if (t->ID == ID) {
            found = GL_TRUE;
            if (i < count - 1) {
                memmove(t, t+1, (count - i - 1) * sizeof(AEFunc));
            }
            count--;
            memset(funcs + count, 0, sizeof(AEFunc));
            
            duk_push_global_stash(ctx);             // stash
            duk_get_prop_string(ctx, -1, kTimer);   // stash kTimer
            duk_del_prop_index(ctx, -1, ID);        // stash kTimer (delete kTimer[ID])
            duk_pop_2(ctx);                         // none
            break;
        }
    }
    duk_push_boolean(ctx, found);
    return 1;
}
