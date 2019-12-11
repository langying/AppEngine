//
//  AEFormat.cpp
//  Amaze
//
//  Created by 韩琼 on 16/2/5.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEFormat.hpp"

//#define _support(name) (std::find(extensions.begin(), extensions.end(), name) != extensions.end())
#define _support(name) AEFindString(extensions, name)

bool AEFindString(std::vector<std::string>& list, std::string item) {
    for (auto it = list.begin(); it != list.end(); it++) {
        if (*it == item) {
            return true;
        }
    }
    return false;
}

std::string AEFormat::styleWithColor(GLuint color) {
    char text[16] = { 0 };
    sprintf(text, "#%X", color);
    return text;
}
std::string AEFormat::mimetype(const char* name, const char* mime) {
    if (mime && strlen(mime) > 4) {
        return mime;
    }
    const char* ext = strrchr(name, '.');
    if (ext) {
        ext++;
        if (strcmp(ext, "mp3") == 0) {
            return "audio/mp3";
        }
        else if (strcmp(ext, "ogg") == 0) {
            return "audio/ogg";
        }
        else if (strcmp(ext, "json") == 0) {
            return "text/json";
        }
        else if (strcmp(ext, "js") == 0 || strcmp(ext, "txt") == 0) {
            return "text/plain";
        }
        else if (strcmp(ext, "jsp") == 0 || strcmp(ext, "htm") == 0 || strcmp(ext, "html") == 0) {
            return "text/html";
        }
        else {
            return "application/octet-stream";
        }
    }
    else {
        return "text/plain";
    }
}

bool AEFormat::isPOT(GLsizei size) {
    return (size & (size-1)) == 0;
}
bool AEFormat::extension(const char* text) {
    static std::vector<std::string> extensions;
    getExtensions(extensions);
    
    bool success = false;
    std::string name = text;
    if (name == "ANGLE_framebuffer_blit") {
        success = _support("GL_EXT_framebuffer_blit");
    }
    else if (name == "ANGLE_framebuffer_multisample") {
        success = _support("GL_EXT_framebuffer_multisample");
    }
    else if (name == "ANGLE_instanced_arrays") {
        success = (_support("GL_ARB_instanced_arrays") || _support("GL_EXT_instanced_arrays"))
        &&(_support("GL_ARB_draw_instanced")   || _support("GL_EXT_draw_instanced"));
    }
    else if (name == "EXT_sRGB") {
#if TARGET_OS_IOS
        success = _support("GL_EXT_sRGB");
#else
        success =  _support("GL_EXT_texture_sRGB")
        &&(_support("GL_EXT_framebuffer_sRGB") || _support("GL_ARB_framebuffer_sRGB"));
#endif
    }
    else if (name == "EXT_frag_depth") {
        success = _support("GL_EXT_frag_depth");
    }
    else if (name == "OES_rgb8_rgba8") {
        success = true;
    }
    else if (name == "OES_texture_float" || name == "OES_texture_half_float" || name == "OES_texture_float_linear" || name == "OES_texture_half_float_linear") {
        success = _support("GL_ARB_texture_float") || _support("GL_OES_texture_float");
    }
    else if (name == "OES_vertex_array_object") {
        success = _support("GL_ARB_vertex_array_object") || _support("GL_OES_vertex_array_object") || _support("GL_APPLE_vertex_array_object");
    }
    else if (name == "OES_standard_derivatives") {
        success = true;
    }
    else if (name == "OES_element_index_uint") {
        success = true;
    }
    else if (name == "EXT_shader_texture_lod") {
        success = _support("GL_EXT_shader_texture_lod");
    }
    else if (name == "EXT_texture_filter_anisotropic") {
        success = _support("GL_EXT_texture_filter_anisotropic");
    }
    else if (name == "EXT_draw_buffers") {
        success = _support("GL_EXT_draw_buffers") || _support("GL_ARB_draw_buffers");
    }
    else if (name == "EXT_packed_depth_stencil") {
        success = _support("GL_OES_packed_depth_stencil") || _support("GL_EXT_packed_depth_stencil");
    }
    else if (name == "WEBGL_compressed_texture_s3tc") {
        success =  _support("GL_EXT_texture_compression_s3tc")
        ||(_support("GL_EXT_texture_compression_dxt1") && _support("GL_CHROMIUM_texture_compression_dxt3") && _support("GL_CHROMIUM_texture_compression_dxt5"));
    }
    else if (name == "WEBKIT_WEBGL_compressed_texture_atc") {
        success = _support("GL_AMD_compressed_ATC_texture");
    }
    else if (name == "WEBGL_compressed_texture_pvrtc") {
        success = _support("GL_IMG_texture_compression_pvrtc");
    }
    else if (name == "WEBGL_depth_texture") {
        success = _support("GL_OES_depth_texture") || _support("GL_ARB_depth_texture") || _support("GL_CHROMIUM_depth_texture");
    }
    else if (name == "WEBGL_debug_shaders") {
        success = _support("GL_ANGLE_translated_shader_source");
    }
    else {
        success = _support(("GL_" + name));
    }
    return success;
}
uint AEFormat::utf8Length(const char* text) {
    uint len = 0;
    for (int idx = 0; text[idx]; idx++) {
        if ((text[idx] & 0xC0) != 0x80) {
            len++;
        }
    }
    return len;
}
void AEFormat::getExtensions(std::vector<std::string>& extensions) {
    if (extensions.size()) {
        return;
    }
    std::string splite = " ";
    std::string source = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    std::string::size_type postion1 = 0;
    std::string::size_type postion2 = source.find(splite);
    while (postion2 != std::string::npos) {
        extensions.push_back(source.substr(postion1, postion2 - postion1));
        postion1 = postion2 + splite.size();
        postion2 = source.find(splite, postion1);
    }
    if (postion1 != source.length()) {
        extensions.push_back(source.substr(postion1));
    }
}

void AEFormat::arrayValueOf(duk_context* ctx, duk_idx_t idx, duk_uarridx_t i, GLint* value) {
    duk_get_prop_index(ctx, idx, i);
    *value = duk_get_int(ctx, -1);
    duk_pop(ctx);
}
void AEFormat::arrayValueOf(duk_context* ctx, duk_idx_t idx, duk_uarridx_t i, GLuint* value) {
    duk_get_prop_index(ctx, idx, i);
    *value = duk_get_uint(ctx, -1);
    duk_pop(ctx);
}
void AEFormat::arrayValueOf(duk_context* ctx, duk_idx_t idx, duk_uarridx_t i, GLfloat* value) {
    duk_get_prop_index(ctx, idx, i);
    *value = duk_get_number(ctx, -1);
    duk_pop(ctx);
}

void AEFormat::convert(void* pixels, uint width, uint height, bool unpackFlipY, bool unpackPremultiplyAlpha) {
    if (pixels && unpackFlipY) {
        GLuint size = width * sizeof(GLuint);
        GLchar *src, *dst;
        GLchar *img = static_cast<GLchar*>(pixels);
        GLchar *buf = static_cast<GLchar*>(malloc(size));
        for (uint idx = 0, len = height / 2; idx < len; idx++) {
            src = img + size * idx;
            dst = img + size * (height - idx - 1);
            memcpy(buf, src, size);
            memcpy(src, dst, size);
            memcpy(dst, buf, size);
        }
        free(buf);
    }
    
    if (pixels && unpackPremultiplyAlpha) {
        // 目前只支持RGBA，其他格式的代码见H5Demos文件夹中的AEFormat
        GLubyte* img = static_cast<GLubyte*>(pixels);
        for (int idx = 0, len = width * height; idx < len; idx += 4) {
            GLfloat size = img[idx + 3] / 255.0f;
            img[idx + 0] = static_cast<GLubyte>(img[idx + 0] * size);
            img[idx + 1] = static_cast<GLubyte>(img[idx + 1] * size);
            img[idx + 2] = static_cast<GLubyte>(img[idx + 2] * size);
            // img[idx + 3] = img[idx + 3];
        }
    }
}
