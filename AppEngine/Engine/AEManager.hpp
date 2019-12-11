//
//  AEManager.hpp
//  Action
//
//  Created by 韩琼 on 16/1/9.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEManager_hpp
#define AEManager_hpp

#include "AEURL.hpp"

typedef struct _AETouch {
    Integer ID;
    GLfloat x, y;
} AETouch;

typedef std::string (*AEManagerHandle)(AEManager* mgr, const char* text);

class AEManager {
public:
    static char KeyUp[];
    static char KeyDown[];
    static char TouchEnd[];
    static char TouchMove[];
    static char TouchStart[];
    static char TouchCancel[];
    static char MouseUp[];
    static char MouseOut[];
    static char MouseMove[];
    static char MouseDown[];
    
    static char* event(AEEvent::Type type);
    static AEManager* find(duk_context* ctx);
    static std::string defaultHandle(AEManager* mgr, const char* text);

public:
    AEURL           _url;
    duk_context*    _ctx;
    AEManagerHandle _handle;
    std::map<Integer, AETouch> _touches;
    
public:
    ~AEManager();
    AEManager(GLuint width, GLuint height, GLfloat scale, const char* agent);
    void init();
    void jsgc();
    void check();
    void setHandle(AEManagerHandle handle);
    bool execute(const char* url);
    void loadURL(const char* url);
    void applyTimer();
    void applyResize(GLuint width, GLuint height);
    void applyKeyboard(AEEvent::Type type, GLint keyCode);
    
    template<typename Handle>
    void merge(AEEvent::Type type, GLint len, AETouch* pts, Handle handle);
    void touch(AEEvent::Type type, GLint len, AETouch* pts);
    void orientation(double alpha, double beta, double gamma);
    
    std::string absURL(const char* name);
    std::string evalute(const char* text);
    
protected:
    js_func(now);
    js_func(alert);
    js_func(speak);
    js_func(handle);
    js_func(execute);
    js_func(garbage);
    js_func(println);
    
    // localStorage
    js_func(delItem);
    js_func(getItem);
    js_func(setItem);

    js_func(escape);
    js_func(unescape);
    js_func(decodeURI);
    js_func(encodeURI);
    js_func(decodeURIComponent);
    js_func(encodeURIComponent);
};

#endif /* AEManager_hpp */
