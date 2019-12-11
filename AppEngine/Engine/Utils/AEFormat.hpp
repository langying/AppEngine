//
//  AEFormat.hpp
//  Amaze
//
//  Created by 韩琼 on 16/2/5.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEFormat_hpp
#define AEFormat_hpp

#include "AEPlatform.hpp"

class AEFormat {
public:
    template<typename Handle>
    static void font(const char* style, Handle handle) {
        GLfloat     size = 0;
        std::string name = "";
        split(style, " ", [&size, &name](uint idx, const char* item, bool* stop) {
            int len = static_cast<int>(strlen(item));
            if (len <= 2) {
            }
            else if (isdigit(item[0])) {
                size = strtof(item, NULL);
            }
            else {
                name = item;
            }
        });
        handle(size, name.empty() ? nullptr : name.c_str());
    }
    
    template<typename Handle>
    static void split(const char* source, const char* token, Handle handle) {
        size_t length = strlen(source);
        char*  buffer = static_cast<char*>(calloc(length + 1, sizeof(char)));
        memcpy(buffer, source, length);
        
        uint  loop = 0;
        bool  stop = false;
        char* item = strtok(buffer, token);
        while(item && !stop) {
            handle(loop, item, &stop);
            item = strtok(nullptr, token);
            loop++;
        }
        free(buffer);
    }

    template<typename GLType, typename Handle>
    static void getTypeBuffer(duk_context* ctx, duk_idx_t idx, Handle handle) {
        if (duk_is_array(ctx, idx)) {
            duk_get_prop_string(ctx, idx, "length");
            GLuint  len = duk_get_int(ctx, -1);
            duk_pop(ctx);
            GLType* buf = static_cast<GLType*>(malloc(len * sizeof(GLType)));
            for (GLuint i = 0; i < len; i++) {
                arrayValueOf(ctx, idx, i, buf+i);
            }
            handle(len, buf);
            free(buf);
        }
        else {
            size_t  len = 0;
            GLvoid* buf = duk_get_buffer_data(ctx, idx, &len);
            handle(static_cast<GLuint>(len) / sizeof(GLType), static_cast<GLType*>(buf));
        }
    }

public:
    static std::string styleWithColor(GLuint color);
    static std::string mimetype(const char* name, const char* mime);

public:
    static bool isPOT(GLsizei size);
    static bool extension(const char* name);
    static uint utf8Length(const char* text);
    static void getExtensions(std::vector<std::string>& extensions);
    
public:
    static void arrayValueOf(duk_context* ctx, duk_idx_t idx, duk_uarridx_t i, GLint* value);
    static void arrayValueOf(duk_context* ctx, duk_idx_t idx, duk_uarridx_t i, GLuint* value);
    static void arrayValueOf(duk_context* ctx, duk_idx_t idx, duk_uarridx_t i, GLfloat* value);
    
public:
    static void convert(void* image, uint width, uint height, bool unpackFlipY, bool unpackPremultiplyAlpha);
};

#endif /* AEFormat_hpp */
