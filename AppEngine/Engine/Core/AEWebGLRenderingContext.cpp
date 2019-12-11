//
//  AEWebGLRenderingContext.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/1/21.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#define ctx_set_gl(key); \
duk_push_uint(ctx, GL_##key);\
duk_put_prop_string(ctx, -2, #key);

#include "AEFormat.hpp"
#include "AEManager.hpp"
#include "AEWebGLRenderingContext.hpp"

AEWebGLRenderingContext::~AEWebGLRenderingContext() {
}
AEWebGLRenderingContext::AEWebGLRenderingContext() : cx(nullptr) {
    fbo       = 0;
    alpha     = true;
    antialias = true;
    depth     = true;
    stencil   = true;
    packAlignment              = 1;
    premultipliedAlpha         = false;
    preserveDrawingBuffer      = false;
    unpackFlipY                = false;
    unpackAlignment            = 1;
    unpackPremultiplyAlpha     = false;
    unpackColorspaceConversion = false;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
}

int AEWebGLRenderingContext::size(duk_context* ctx) {
    GLuint width  = duk_get_uint(ctx, 0);
    GLuint height = duk_get_uint(ctx, 1);
    LOG("AEWebGLRenderingContext::size(%d, %d)", width, height);
    return 0;
}

int AEWebGLRenderingContext::clearDepth(duk_context* ctx) {
    return clearDepthf(ctx);
}
int AEWebGLRenderingContext::createBuffer(duk_context* ctx) {
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    duk_push_uint(ctx, buffer);
    return 1;
}
int AEWebGLRenderingContext::createFramebuffer(duk_context* ctx) {
    GLuint buffer = 0;
    glGenFramebuffers(1, &buffer);
    duk_push_uint(ctx, buffer);
    return 1;
}
int AEWebGLRenderingContext::createRenderbuffer(duk_context* ctx) {
    GLuint buffer = 0;
    glGenRenderbuffers(1, &buffer);
    duk_push_uint(ctx, buffer);
    return 1;
}
int AEWebGLRenderingContext::createTexture(duk_context* ctx) {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    duk_push_uint(ctx, texture);
    return 1;
}
int AEWebGLRenderingContext::deleteBuffer(duk_context* ctx) {
    GLuint buffer = duk_get_uint(ctx, 0);
    glDeleteBuffers(1, &buffer);
    return 0;
}
int AEWebGLRenderingContext::deleteFramebuffer(duk_context* ctx) {
    GLuint buffer = duk_get_uint(ctx, 0);
    glDeleteFramebuffers(1, &buffer);
    return 0;
}
int AEWebGLRenderingContext::deleteRenderbuffer(duk_context* ctx) {
    GLuint buffer = duk_get_uint(ctx, 0);
    glDeleteRenderbuffers(1, &buffer);
    return 0;
}
int AEWebGLRenderingContext::deleteTexture(duk_context* ctx) {
    GLuint texture = duk_get_uint(ctx, 0);
    glDeleteTextures(1, &texture);
    LOG("delete texture: %d", texture);
    return 0;
}
int AEWebGLRenderingContext::depthRange(duk_context* ctx) {
    return depthRangef(ctx);
}
int AEWebGLRenderingContext::getBufferParameter(duk_context* ctx) {
    return getBufferParameteriv(ctx);
}
int AEWebGLRenderingContext::getContextAttributes(duk_context* ctx) {
    js_thiz(AEWebGLRenderingContext);
    duk_push_object(ctx);
    js_set_bool("alpha", thiz->alpha);
    js_set_bool("antialias", thiz->antialias);
    js_set_bool("depth", thiz->depth);
    js_set_bool("stencil", thiz->stencil);
    js_set_bool("premultipliedAlpha", thiz->premultipliedAlpha);
    js_set_bool("preserveDrawingBuffer", thiz->preserveDrawingBuffer);
    return 1;
}
int AEWebGLRenderingContext::getExtension(duk_context* ctx) {
    const char* name = duk_get_string(ctx, 0);
    if (strcmp(name, "WEBGL_debug_renderer_info") == 0) {
    	duk_push_object(ctx);
    	ctx_set_gl(UNMASKED_VENDOR_WEBGL);
    	ctx_set_gl(UNMASKED_RENDERER_WEBGL);
    }
    else {
    	// bool flag = AEFormat::extension(name);
    	// duk_push_boolean(ctx, flag);
    	duk_push_null(ctx);
    }
    return 1;
}
int AEWebGLRenderingContext::getFramebufferAttachmentParameter(duk_context* ctx) {
    return getFramebufferAttachmentParameteriv(ctx);
}
int AEWebGLRenderingContext::getParameter(duk_context* ctx) {
    js_thiz(AEWebGLRenderingContext);
    GLenum pname = duk_get_uint(ctx, 0);
    switch(pname) {
        case GL_BLEND:
        case GL_CULL_FACE:
        case GL_DEPTH_TEST:
        case GL_DEPTH_WRITEMASK:
        case GL_DITHER:
        case GL_POLYGON_OFFSET_FILL:
        case GL_SAMPLE_COVERAGE_INVERT:
        case GL_SCISSOR_TEST:
        case GL_STENCIL_TEST: {
        	GLboolean ret = GL_FALSE;
        	glGetBooleanv(pname, &ret);
        	duk_push_boolean(ctx, ret);
        	break;
        }
        case GL_UNMASKED_VENDOR_WEBGL : {
        	const char* ret = (const char*)glGetString(GL_VENDOR);
        	duk_push_string(ctx, ret);
        	break;
        }
        case GL_UNMASKED_RENDERER_WEBGL: {
        	const char* ret = (const char*)glGetString(GL_RENDERER);
        	duk_push_string(ctx, ret);
        	break;
        }
        case GL_UNPACK_FLIP_Y_WEBGL: {
            duk_push_boolean(ctx, thiz->unpackFlipY);
            break;
        }
        case GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL: {
            duk_push_uint(ctx, thiz->unpackPremultiplyAlpha);
            break;
        }
        case GL_UNPACK_COLORSPACE_CONVERSION_WEBGL: {
            duk_push_uint(ctx, thiz->unpackColorspaceConversion);
            break;
        }
        case GL_DEPTH_CLEAR_VALUE:
        case GL_LINE_WIDTH:
        case GL_POLYGON_OFFSET_FACTOR:
        case GL_POLYGON_OFFSET_UNITS:
        case GL_SAMPLE_COVERAGE_VALUE: {
            GLfloat param;
            glGetFloatv(pname, &param);
            duk_push_number(ctx, param);
            break;
        }
        case GL_RENDERER:
        case GL_SHADING_LANGUAGE_VERSION:
        case GL_VENDOR:
        case GL_VERSION:
        case GL_EXTENSIONS: {
            const char* buffer = (const char*)glGetString(pname);
            duk_push_string(ctx, buffer ?: "");
            break;
        }
        case GL_MAX_VIEWPORT_DIMS: {
            GLint params[2];
            glGetIntegerv(pname, params);
            duk_idx_t idx = duk_push_array(ctx);
            for (int i = 0; i < 2; i++) {
                duk_push_int(ctx, params[i]);
                duk_put_prop_index(ctx, idx, i);
            }
            break;
        }
        case GL_SCISSOR_BOX:
        case GL_VIEWPORT: {
            GLint params[4];
            glGetIntegerv(pname, params);
            duk_idx_t idx = duk_push_array(ctx);
            for (int i = 0; i < 4; i++) {
                duk_push_int(ctx, params[i]);
                duk_put_prop_index(ctx, idx, i);
            }
            break;
        }
        case GL_ALIASED_LINE_WIDTH_RANGE:
        case GL_ALIASED_POINT_SIZE_RANGE:
        case GL_DEPTH_RANGE: {
            GLfloat params[2];
            glGetFloatv(pname, params);
            duk_idx_t idx = duk_push_array(ctx);
            for (int i = 0; i < 2; i++) {
                duk_push_number(ctx, params[i]);
                duk_put_prop_index(ctx, idx, i);
            }
            break;
        }
        case GL_BLEND_COLOR:
        case GL_COLOR_CLEAR_VALUE: {
            GLfloat params[4];
            glGetFloatv(pname, params);
            duk_idx_t idx = duk_push_array(ctx);
            for (int i = 0; i < 4; i++) {
                duk_push_number(ctx, params[i]);
                duk_put_prop_index(ctx, idx, i);
            }
            break;
        }
        case GL_COLOR_WRITEMASK: {
            GLboolean params[4];
            glGetBooleanv(pname, params);
            duk_idx_t idx = duk_push_array(ctx);
            for (int i = 0; i < 4; i++) {
                duk_push_boolean(ctx, params[i]);
                duk_put_prop_index(ctx, idx, i);
            }
            break;
        }
        case GL_ARRAY_BUFFER_BINDING:
        case GL_CURRENT_PROGRAM:
        case GL_ELEMENT_ARRAY_BUFFER_BINDING:
        case GL_FRAMEBUFFER_BINDING:
        case GL_RENDERBUFFER_BINDING:
        case GL_TEXTURE_BINDING_2D:
        case GL_TEXTURE_BINDING_CUBE_MAP: {
            GLint param;
            glGetIntegerv(pname, &param);
            duk_push_int(ctx, param);
            break;
        }
        default: {
            GLint param;
            glGetIntegerv(pname, &param);
            duk_push_int(ctx, param);
            break;
        }
    }
    return 1;
}
int AEWebGLRenderingContext::getProgramParameter(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    GLuint pname   = duk_get_uint(ctx, 1);
    GLint  param   = 0;
    glGetProgramiv(program, pname, &param);
    switch (pname) {
        case GL_DELETE_STATUS:
        case GL_LINK_STATUS:
        case GL_VALIDATE_STATUS: {
            duk_push_boolean(ctx, param);
            break;
        }
        case GL_ATTACHED_SHADERS:
        case GL_ACTIVE_ATTRIBUTES:
        case GL_ACTIVE_UNIFORMS: {
            duk_push_int(ctx, param);
            break;
        }
        default: {
            duk_push_int(ctx, param);
            break;
        }
    }
    return 1;
}
int AEWebGLRenderingContext::getRenderbufferParameter(duk_context* ctx) {
    return getRenderbufferParameteriv(ctx);
}
int AEWebGLRenderingContext::getShaderParameter(duk_context* ctx) {
    return getShaderiv(ctx);
}
int AEWebGLRenderingContext::getSupportedExtensions(duk_context* ctx) {
    std::vector<std::string> extensions;
    AEFormat::getExtensions(extensions);
    
    duk_idx_t idx = duk_push_array(ctx);
    for (size_t i = 0, size = extensions.size(); i < size; i++) {
        duk_push_string(ctx, extensions[i].c_str());
        duk_put_prop_index(ctx, idx, static_cast<int>(i));
    }
    return 1;
}
int AEWebGLRenderingContext::getTexParameter(duk_context* ctx) {
    return getTexParameteriv(ctx);
}
int AEWebGLRenderingContext::getUniform(duk_context* ctx) {
    GLuint  program    = duk_get_uint(ctx, 0);
    GLint   location   = duk_get_uint(ctx, 1);
    GLfloat params[16] = { 0 };
    glGetUniformfv(program, location, params);
    duk_idx_t idx = duk_push_array(ctx);
    for (int i = 0; i < 16; i++) {
        duk_push_number(ctx, params[i]);
        duk_put_prop_index(ctx, idx, i);
    }
    return 1;
}
int AEWebGLRenderingContext::getVertexAttrib(duk_context* ctx) {
    GLuint index = duk_get_uint(ctx, 0);
    GLuint pname = duk_get_uint(ctx, 1);
    GLint  param = 0;
    switch(pname) {
        case GL_VERTEX_ATTRIB_ARRAY_ENABLED:
        case GL_VERTEX_ATTRIB_ARRAY_NORMALIZED: {
            glGetVertexAttribiv(index, pname, &param);
            duk_push_boolean(ctx, param);
            break;
        }
        case GL_VERTEX_ATTRIB_ARRAY_SIZE:
        case GL_VERTEX_ATTRIB_ARRAY_STRIDE:
        case GL_VERTEX_ATTRIB_ARRAY_TYPE:
        case GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING: {
            glGetVertexAttribiv(index, pname, &param);
            duk_push_int(ctx, param);
            break;
        }
        case GL_CURRENT_VERTEX_ATTRIB: {
            GLfloat vextex_attribs[4];
            glGetVertexAttribfv(index, pname, vextex_attribs);
            duk_idx_t idx = duk_push_array(ctx);
            for (int i = 0; i < 4; i++) {
                duk_push_number(ctx, vextex_attribs[i]);
                duk_put_prop_index(ctx, idx, i);
            }
            break;
        }
        default: {
            duk_error(ctx, DUK_ERR_RANGE_ERROR, "GetVertexAttrib: Invalid Enum[%d, %d]", index, pname);
            break;
        }
    }
    return 1;
}
int AEWebGLRenderingContext::getVertexAttribOffset(duk_context* ctx) {
    return getVertexAttribPointerv(ctx);
}
int AEWebGLRenderingContext::isContextLost(duk_context* ctx) {
    duk_push_boolean(ctx, GL_FALSE);
    return 1;
}

int AEWebGLRenderingContext::activeTexture(duk_context* ctx) {
    GLenum texture = duk_get_uint(ctx, 0);
    glActiveTexture(texture);
    return 0;
}
int AEWebGLRenderingContext::attachShader(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    GLuint shader  = duk_get_uint(ctx, 1);
    glAttachShader(program, shader);
    return 0;
}
int AEWebGLRenderingContext::bindAttribLocation(duk_context* ctx) {
    const GLuint  program = duk_get_uint(ctx, 0);
    const GLuint  index   = duk_get_uint(ctx, 1);
    const GLchar* pname   = duk_get_string(ctx, 2);
    glBindAttribLocation(program, index, pname);
    return 0;
}
int AEWebGLRenderingContext::bindBuffer(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLuint buffer = duk_get_uint(ctx, 1);
    glBindBuffer(target, buffer);
    return 0;
}
int AEWebGLRenderingContext::bindFramebuffer(duk_context* ctx) {
    js_thiz(AEWebGLRenderingContext);
    GLenum target = duk_get_uint(ctx, 0);
    GLuint buffer = duk_get_uint(ctx, 1);
    if (!buffer) {
        buffer = thiz->fbo;
    }
    glBindFramebuffer(target, buffer);
    return 0;
}
int AEWebGLRenderingContext::bindRenderbuffer(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLuint buffer = duk_get_uint(ctx, 1);
    glBindRenderbuffer(target, buffer);
    return 0;
}
int AEWebGLRenderingContext::bindTexture(duk_context* ctx) {
    GLenum target  = duk_get_uint(ctx, 0);
    GLuint texture = duk_get_uint(ctx, 1);
    glBindTexture(target, texture);
    return 0;
}
int AEWebGLRenderingContext::blendColor(duk_context* ctx) {
    GLfloat r = duk_get_number(ctx, 0);
    GLfloat g = duk_get_number(ctx, 1);
    GLfloat b = duk_get_number(ctx, 2);
    GLfloat a = duk_get_number(ctx, 3);
    glBlendColor(r, g, b, a);
    return 0;
}
int AEWebGLRenderingContext::blendEquation(duk_context* ctx) {
    GLenum mode = duk_get_uint(ctx, 0);
    glBlendEquation(mode);
    return 0;
}
int AEWebGLRenderingContext::blendEquationSeparate(duk_context* ctx) {
    GLenum modeRGB   = duk_get_uint(ctx, 0);
    GLenum modeAlpha = duk_get_uint(ctx, 1);
    glBlendEquationSeparate(modeRGB, modeAlpha);
    return 0;
}
int AEWebGLRenderingContext::blendFunc(duk_context* ctx) {
    GLenum sfactor = duk_get_uint(ctx, 0);
    GLenum dfactor = duk_get_uint(ctx, 1);
    glBlendFunc(sfactor, dfactor);
    return 0;
}
int AEWebGLRenderingContext::blendFuncSeparate(duk_context* ctx){
    GLenum srcRGB   = duk_get_uint(ctx, 0);
    GLenum dstRGB   = duk_get_uint(ctx, 1);
    GLenum srcAlpha = duk_get_uint(ctx, 2);
    GLenum dstAlpha = duk_get_uint(ctx, 3);
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    return 0;
}
int AEWebGLRenderingContext::bufferData(duk_context* ctx) {
    if (duk_is_object(ctx, 1)) {
        GLenum  target  = duk_get_uint(ctx, 0);
        size_t  length  = 0;
        GLvoid* buffer  = duk_get_buffer_data(ctx, 1, &length);
        GLenum  usage   = duk_get_uint(ctx, 2);
        glBufferData(target, length, buffer, usage);
    }
    else {
        GLenum     target  = duk_get_uint(ctx, 0);
        GLsizeiptr length  = duk_get_uint(ctx, 1);
        GLvoid*    buffer  = nullptr;
        GLenum     usage   = duk_get_uint(ctx, 2);
        glBufferData(target, length, buffer, usage);
    }
    return 0;
}
int AEWebGLRenderingContext::bufferSubData(duk_context* ctx) {
    GLenum   target  = duk_get_uint(ctx, 0);
    GLintptr offset  = duk_get_int( ctx, 1);
    size_t   length  = 0;
    GLvoid*  buffer  = duk_get_buffer_data(ctx, 2, &length);
    glBufferSubData(target, offset, length, buffer);
    return 0;
}
int AEWebGLRenderingContext::checkFramebufferStatus(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum status = glCheckFramebufferStatus(target);
    duk_push_uint(ctx, status);
    return 1;
}
int AEWebGLRenderingContext::clear(duk_context* ctx) {
    GLbitfield mask = duk_get_uint(ctx, 0);
    glClear(mask);
    return 0;
}
int AEWebGLRenderingContext::clearColor(duk_context* ctx) {
    GLfloat r = duk_get_number(ctx, 0);
    GLfloat g = duk_get_number(ctx, 1);
    GLfloat b = duk_get_number(ctx, 2);
    GLfloat a = duk_get_number(ctx, 3);
    glClearColor(r, g, b, a);
    return 0;
}
int AEWebGLRenderingContext::clearDepthf(duk_context* ctx) {
    GLclampf depth = duk_get_number(ctx, 0);
    glClearDepthf(depth);
    return 0;
}
int AEWebGLRenderingContext::clearStencil(duk_context* ctx) {
    GLint s = duk_get_int(ctx, 0);
    glClearStencil(s);
    return 0;
}
int AEWebGLRenderingContext::colorMask(duk_context* ctx) {
    GLboolean r = duk_get_boolean(ctx, 0);
    GLboolean g = duk_get_boolean(ctx, 1);
    GLboolean b = duk_get_boolean(ctx, 2);
    GLboolean a = duk_get_boolean(ctx, 3);
    glColorMask(r, g, b, a);
    return 0;
}
int AEWebGLRenderingContext::compileShader(duk_context* ctx) {
    GLuint shader = duk_get_uint(ctx, 0);
    glCompileShader(shader);
    return 0;
}
int AEWebGLRenderingContext::compressedTexImage2D(duk_context* ctx) {
    GLenum  target = duk_get_uint(ctx, 0);
    GLint   level  = duk_get_int(ctx, 1);
    GLenum  format = duk_get_uint(ctx, 2);
    GLsizei width  = duk_get_int(ctx, 3);
    GLsizei height = duk_get_int(ctx, 4);
    GLint   border = duk_get_int(ctx, 5);
    size_t  length = 0;
    GLvoid* buffer = duk_get_buffer_data(ctx, 6, &length);
    glCompressedTexImage2D(target, level, format, width, height, border, (GLsizei)length, buffer);
    return 0;
}
int AEWebGLRenderingContext::compressedTexSubImage2D(duk_context* ctx) {
    GLenum  target  = duk_get_uint(ctx, 0);
    GLint   level   = duk_get_int(ctx, 1);
    GLint   xoffset = duk_get_int(ctx, 2);
    GLint   yoffset = duk_get_int(ctx, 3);
    GLsizei width   = duk_get_int(ctx, 4);
    GLsizei height  = duk_get_int(ctx, 5);
    GLenum  format  = duk_get_uint(ctx, 6);
    size_t  length  = 0;
    GLvoid* buffer  = duk_get_buffer_data(ctx, 7, &length);
    glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, static_cast<GLint>(length), buffer);
    return 0;
}
int AEWebGLRenderingContext::copyTexImage2D(duk_context* ctx) {
    GLenum  target  = duk_get_uint(ctx, 0);
    GLint   level   = duk_get_int(ctx, 1);
    GLenum  infmt   = duk_get_uint(ctx, 2);
    GLint   x       = duk_get_int(ctx, 3);
    GLint   y       = duk_get_int(ctx, 4);
    GLsizei w       = duk_get_int(ctx, 5);
    GLsizei h       = duk_get_int(ctx, 6);
    GLint   border  = duk_get_int(ctx, 7);
    glCopyTexImage2D(target, level, infmt, x, y, w, h, border);
    return 0;
}
int AEWebGLRenderingContext::copyTexSubImage2D(duk_context* ctx) {
    GLenum  target  = duk_get_uint(ctx, 0);
    GLint   level   = duk_get_int(ctx, 1);
    GLint   xoffset = duk_get_int(ctx, 2);
    GLint   yoffset = duk_get_int(ctx, 3);
    GLint   x       = duk_get_int(ctx, 4);
    GLint   y       = duk_get_int(ctx, 5);
    GLsizei width   = duk_get_int(ctx, 6);
    GLsizei height  = duk_get_int(ctx, 7);
    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    return 0;
}
int AEWebGLRenderingContext::createProgram(duk_context* ctx) {
    GLuint program = glCreateProgram();
    duk_push_uint(ctx, program);
    return 1;
}
int AEWebGLRenderingContext::createShader(duk_context* ctx) {
    GLenum type   = duk_get_uint(ctx, 0);
    GLuint shader = glCreateShader(type);
    duk_push_uint(ctx, shader);
    return 1;
}
int AEWebGLRenderingContext::cullFace(duk_context* ctx) {
    GLenum mode = duk_get_uint(ctx, 0);
    glCullFace(mode);
    return 0;
}
int AEWebGLRenderingContext::deleteBuffers(duk_context* ctx) {
    AEFormat::getTypeBuffer<GLuint>(ctx, 0, [](GLint count, GLuint* value) {
        glDeleteBuffers(count, value);
    });
    return 0;
}
int AEWebGLRenderingContext::deleteFramebuffers(duk_context* ctx) {
    AEFormat::getTypeBuffer<GLuint>(ctx, 0, [](GLint count, GLuint* value) {
        glDeleteFramebuffers(count, value);
    });
    return 0;
}
int AEWebGLRenderingContext::deleteProgram(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    glDeleteProgram(program);
    return 0;
}
int AEWebGLRenderingContext::deleteRenderbuffers(duk_context* ctx) {
    AEFormat::getTypeBuffer<GLuint>(ctx, 0, [](GLint count, GLuint* value) {
        glDeleteRenderbuffers(count, value);
    });
    return 0;
}
int AEWebGLRenderingContext::deleteShader(duk_context* ctx) {
    GLuint shader = duk_get_uint(ctx, 0);
    glDeleteShader(shader);
    return 0;
}
int AEWebGLRenderingContext::deleteTextures(duk_context* ctx) {
    AEFormat::getTypeBuffer<GLuint>(ctx, 0, [](GLint count, GLuint* value) {
        glDeleteTextures(count, value);
    });
    return 0;
}
int AEWebGLRenderingContext::depthFunc(duk_context* ctx) {
    GLenum func = duk_get_uint(ctx, 0);
    glDepthFunc(func);
    return 0;
}
int AEWebGLRenderingContext::depthMask(duk_context* ctx) {
    GLboolean flag = duk_get_boolean(ctx, 0);
    glDepthMask(flag);
    return 0;
}
int AEWebGLRenderingContext::depthRangef(duk_context* ctx) {
    GLclampf zNear = duk_get_number(ctx, 0);
    GLclampf zFar  = duk_get_number(ctx, 1);
    glDepthRangef(zNear, zFar);
    return 0;
}
int AEWebGLRenderingContext::detachShader(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    GLuint shader  = duk_get_uint(ctx, 1);
    glDetachShader(program, shader);
    return 0;
}
int AEWebGLRenderingContext::disable(duk_context* ctx) {
    GLenum cap = duk_get_uint(ctx, 0);
    glDisable(cap);
    return 0;
}
int AEWebGLRenderingContext::disableVertexAttribArray(duk_context* ctx) {
    GLuint index = duk_get_uint(ctx, 0);
    glDisableVertexAttribArray(index);
    return 0;
}
int AEWebGLRenderingContext::drawArrays(duk_context* ctx) {
    GLenum  mode  = duk_get_uint(ctx, 0);
    GLint   first = duk_get_int(ctx, 1);
    GLsizei count = duk_get_int(ctx, 2);
    glDrawArrays(mode, first, count);
    return 0;
}
int AEWebGLRenderingContext::drawElements(duk_context* ctx) {
    GLenum  mode    = duk_get_uint(ctx, 0);
    GLsizei count   = duk_get_int(ctx, 1);
    GLenum  type    = duk_get_uint(ctx, 2);
    GLuint  indices = duk_get_uint(ctx, 3);
    glDrawElements(mode, count, type, reinterpret_cast<const GLvoid*>(indices));
    return 0;
}
int AEWebGLRenderingContext::enable(duk_context* ctx) {
    GLenum cap = duk_get_uint(ctx, 0);
    glEnable(cap);
    return 0;
}
int AEWebGLRenderingContext::enableVertexAttribArray(duk_context* ctx) {
    GLuint index = duk_get_uint(ctx, 0);
    glEnableVertexAttribArray(index);
    return 0;
}
int AEWebGLRenderingContext::finish(duk_context* ctx) {
    glFinish();
    return 0;
}
int AEWebGLRenderingContext::flush(duk_context* ctx) {
    glFlush();
    return 0;
}
int AEWebGLRenderingContext::framebufferRenderbuffer(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum attach = duk_get_uint(ctx, 1);
    GLenum render = duk_get_uint(ctx, 2);
    GLuint buffer = duk_get_uint(ctx, 3);
    if (attach == GL_DEPTH_STENCIL_ATTACHMENT) {
        glFramebufferRenderbuffer(target, GL_DEPTH_ATTACHMENT,   render, buffer);
        glFramebufferRenderbuffer(target, GL_STENCIL_ATTACHMENT, render, buffer);
    }
    else {
        glFramebufferRenderbuffer(target, attach, render, buffer);
    }
    return 0;
}
int AEWebGLRenderingContext::framebufferTexture2D(duk_context* ctx) {
    GLenum target  = duk_get_uint(ctx, 0);
    GLenum attach  = duk_get_uint(ctx, 1);
    GLenum textype = duk_get_uint(ctx, 2);
    GLuint texture = duk_get_uint(ctx, 3);
    GLint  level   = duk_get_int(ctx, 4);
    if (attach == GL_DEPTH_STENCIL_ATTACHMENT) {
        glFramebufferTexture2D(target, GL_DEPTH_ATTACHMENT,   textype, texture, level);
        glFramebufferTexture2D(target, GL_STENCIL_ATTACHMENT, textype, texture, level);
    }
    else {
        glFramebufferTexture2D(target, attach, textype, texture, level);
    }
    return 0;
}
int AEWebGLRenderingContext::frontFace(duk_context* ctx) {
    GLenum mode = duk_get_uint(ctx, 0);
    glFrontFace(mode);
    return 0;
}
int AEWebGLRenderingContext::genBuffers(duk_context* ctx) {
    return createBuffer(ctx);
}
int AEWebGLRenderingContext::generateMipmap(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    glGenerateMipmap(target);
    return 0;
}
int AEWebGLRenderingContext::genFramebuffers(duk_context* ctx) {
    return createFramebuffer(ctx);
}
int AEWebGLRenderingContext::genRenderbuffers(duk_context* ctx) {
    return createRenderbuffer(ctx);
}
int AEWebGLRenderingContext::genTextures(duk_context* ctx) {
    return createTexture(ctx);
}
int AEWebGLRenderingContext::getActiveAttrib(duk_context* ctx) {
    GLuint  program = duk_get_uint(ctx, 0);
    GLuint  index   = duk_get_uint(ctx, 1);
    GLsizei bufsize = 0;
    glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufsize);
    
    GLsizei len  = 0;
    GLint   size = 0;
    GLenum  type = 0;
    GLchar* name = (GLchar*)malloc(bufsize + 1);
    glGetActiveAttrib(program, index, bufsize, &len, &size, &type, name);
    
    duk_push_object(ctx);
    js_set_int("size", size);
    js_set_uint("type", type);
    js_set_strs("name", name);
    free(name);
    return 1;
}
int AEWebGLRenderingContext::getActiveUniform(duk_context* ctx) {
    GLuint  program = duk_get_uint(ctx, 0);
    GLuint  index   = duk_get_uint(ctx, 1);
    GLsizei bufsize = 0;
    glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufsize);
    
    GLsizei len  = 0;
    GLint   size = 0;
    GLenum  type = 0;
    GLchar* name = (GLchar*)malloc(bufsize + 1);
    glGetActiveUniform(program, index, bufsize, &len, &size, &type, name);
    
    duk_push_object(ctx);
    js_set_int("size", size);
    js_set_uint("type", type);
    js_set_strs("name", name);
    free(name);
    return 1;
}
int AEWebGLRenderingContext::getAttachedShaders(duk_context* ctx) {
    GLuint  program = duk_get_uint(ctx, 0);
    GLsizei count   = 0;
    GLuint  shaders[8];
    glGetAttachedShaders(program, 8, &count, shaders);
    
    duk_idx_t idx = duk_push_array(ctx);
    for (int i = 0; i < count; i++) {
        duk_push_uint(ctx, shaders[i]);
        duk_put_prop_index(ctx, idx, i);
    }
    return 1;
}
int AEWebGLRenderingContext::getAttribLocation(duk_context* ctx) {
    const GLuint program = duk_get_uint(ctx, 0);
    const GLchar *name   = duk_get_string(ctx, 1);
    const GLint  index   = glGetAttribLocation(program, name);
    duk_push_int(ctx, index);
    return 1;
}
int AEWebGLRenderingContext::getBooleanv(duk_context* ctx) {
    GLenum    pname = duk_get_uint(ctx, 0);
    GLboolean param = GL_FALSE;
    glGetBooleanv(pname, &param);
    duk_push_boolean(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getBufferParameteriv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum pname  = duk_get_uint(ctx, 1);
    GLint  param  = 0;
    glGetBufferParameteriv(target, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getError(duk_context* ctx) {
    GLenum error = glGetError();
    duk_push_uint(ctx, error);
    return 1;
}
int AEWebGLRenderingContext::getFloatv(duk_context* ctx) {
    GLenum  pname = duk_get_uint(ctx, 0);
    GLfloat param = 0;
    glGetFloatv(pname, &param);
    duk_push_number(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getFramebufferAttachmentParameteriv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum attach = duk_get_uint(ctx, 1);
    GLenum pname  = duk_get_uint(ctx, 2);
    GLint  param  = 0;
    if (attach == GL_DEPTH_STENCIL_ATTACHMENT) {
        attach = GL_DEPTH_ATTACHMENT; // Or STENCIL_ATTACHMENT, either works.
    }
    glGetFramebufferAttachmentParameteriv(target, attach, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getIntegerv(duk_context* ctx) {
    GLenum pname = duk_get_uint(ctx, 0);
    GLint  param = 0;
    glGetIntegerv(pname, &param);
    switch (pname) {
#if !TARGET_OS_IOS
        case GL_MAX_FRAGMENT_UNIFORM_VECTORS:
        case GL_MAX_VERTEX_UNIFORM_VECTORS:
        case GL_MAX_VARYING_VECTORS:
            param /= 4;
            break;
#endif
        default:
            break;
    }
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getProgramiv(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    GLenum pname   = duk_get_uint(ctx, 1);
    GLint  param   = 0;
    glGetProgramiv(program, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getProgramInfoLog(duk_context* ctx) {
    GLint  bufsize = 0;
    GLuint program = duk_get_uint(ctx, 0);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufsize);
    
    GLint   count  = 0;
    GLchar* buffer = (GLchar*)malloc(bufsize + 1);
    glGetProgramInfoLog(program, bufsize, &count, buffer);
    duk_push_string(ctx, buffer);
    free(buffer);
    return 1;
}
int AEWebGLRenderingContext::getRenderbufferParameteriv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum pname  = duk_get_uint(ctx, 1);
    GLint  param  = 0;
    glGetRenderbufferParameteriv(target, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getShaderiv(duk_context* ctx) {
    GLuint shader = duk_get_uint(ctx, 0);
    GLenum pname  = duk_get_uint(ctx, 1);
    GLint  param  = 0;
    glGetShaderiv(shader, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getShaderInfoLog(duk_context* ctx) {
    GLint  bufsize = 0;
    GLuint shader  = duk_get_uint(ctx, 0);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufsize);
    
    GLsizei length = 0;
    GLchar* buffer = (GLchar*)malloc(bufsize + 1);
    glGetShaderInfoLog(shader, bufsize, &length, buffer);
    duk_push_string(ctx, buffer);
    free(buffer);
    return 1;
}
int AEWebGLRenderingContext::getShaderPrecisionFormat(duk_context* ctx) {
    GLenum shadertype    = duk_get_uint(ctx, 0);
    GLenum precisiontype = duk_get_uint(ctx, 1);
    GLint  range[2]      = { 0 };
    GLint  precision     = 0;
    glGetShaderPrecisionFormat(shadertype, precisiontype, range, &precision);
    switch (precisiontype) {
        case GL_LOW_INT:
        case GL_MEDIUM_INT:
        case GL_HIGH_INT:
            // 32-bit twos-complement integer format
            range[0]  = 31;
            range[1]  = 30;
            precision = 0;
            break;
        case GL_LOW_FLOAT:
        case GL_MEDIUM_FLOAT:
        case GL_HIGH_FLOAT:
            // IEEE single-precision floating-point format
            range[0]  = 127;
            range[1]  = 127;
            precision = 23;
            break;
        default:
            break;
    }
    
    duk_push_object(ctx);
    duk_push_int(ctx, range[0]);
    duk_put_prop_string(ctx, -2, "rangeMin");
    duk_push_int(ctx, range[1]);
    duk_put_prop_string(ctx, -2, "rangeMax");
    duk_push_int(ctx, precision);
    duk_put_prop_string(ctx, -2, "precision");
    return 1;
}
int AEWebGLRenderingContext::getShaderSource(duk_context* ctx) {
    GLint  length = 0;
    GLuint shader = duk_get_uint(ctx, 0);
    glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &length);
    
    GLchar* source = (GLchar*)malloc(length + 1);
    glGetShaderSource(shader, length, nullptr, source);
    duk_push_string(ctx, source);
    free(source);
    return 1;
}
int AEWebGLRenderingContext::getString(duk_context* ctx) {
    GLenum  name = duk_get_uint(ctx, 0);
    GLchar* text = (GLchar*)glGetString(name);
    duk_push_string(ctx, text);
    return 1;
}
int AEWebGLRenderingContext::getTexParameterfv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLfloat param = 0;
    glGetTexParameterfv(target, pname, &param);
    duk_push_number(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getTexParameteriv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLint   param = 0;
    glGetTexParameteriv(target, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getUniformfv(duk_context* ctx) {
    GLuint  program  = duk_get_uint(ctx, 0);
    GLint   location = duk_get_int(ctx, 1);
    GLfloat param    = 0;
    glGetUniformfv(program, location, &param);
    duk_push_number(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getUniformiv(duk_context* ctx) {
    GLuint program  = duk_get_uint(ctx, 0);
    GLint  location = duk_get_int(ctx, 1);
    GLint  param    = 0;
    glGetUniformiv(program, location, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getUniformLocation(duk_context* ctx) {
    const GLuint  program  = duk_get_uint(ctx, 0);
    const GLchar* pname    = duk_get_string(ctx, 1);
    const GLint   location = glGetUniformLocation(program, pname);
    duk_push_int(ctx, location);
    return 1;
}
int AEWebGLRenderingContext::getVertexAttribfv(duk_context* ctx) {
    GLuint  index = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLfloat param = 0;
    glGetVertexAttribfv(index, pname, &param);
    duk_push_number(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getVertexAttribiv(duk_context* ctx) {
    GLuint index = duk_get_uint(ctx, 0);
    GLenum pname = duk_get_uint(ctx, 1);
    GLint  param = 0;
    glGetVertexAttribiv(index, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::getVertexAttribPointerv(duk_context* ctx) {
    GLuint  index = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLvoid* rtptr   = nullptr;
    glGetVertexAttribPointerv(index, pname, &rtptr);
    duk_push_pointer(ctx, rtptr);
    return 1;
}
int AEWebGLRenderingContext::hint(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum mode   = duk_get_uint(ctx, 1);
    glHint(target, mode);
    return 0;
}
int AEWebGLRenderingContext::isBuffer(duk_context* ctx) {
    GLuint buffer = duk_get_uint(ctx, 0);
    GLboolean flag = glIsBuffer(buffer);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::isEnabled(duk_context* ctx) {
    GLenum cap = duk_get_uint(ctx, 0);
    GLboolean flag = glIsEnabled(cap);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::isFramebuffer(duk_context* ctx) {
    GLuint framebuffer = duk_get_uint(ctx, 0);
    GLboolean flag = glIsFramebuffer(framebuffer);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::isProgram(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    GLboolean flag = glIsProgram(program);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::isRenderbuffer(duk_context* ctx) {
    GLuint renderbuffer = duk_get_uint(ctx, 0);
    GLboolean flag = glIsRenderbuffer(renderbuffer);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::isShader(duk_context* ctx) {
    GLuint shader = duk_get_uint(ctx, 0);
    GLboolean flag = glIsShader(shader);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::isTexture(duk_context* ctx) {
    GLuint texture = duk_get_uint(ctx, 0);
    GLboolean flag = glIsTexture(texture);
    duk_push_boolean(ctx, flag);
    return 1;
}
int AEWebGLRenderingContext::lineWidth(duk_context* ctx) {
    GLfloat width = duk_get_number(ctx, 0);
    glLineWidth(width);
    return 0;
}
int AEWebGLRenderingContext::linkProgram(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    glLinkProgram(program);
    return 0;
}
int AEWebGLRenderingContext::pixelStorei(duk_context* ctx) {
    js_thiz(AEWebGLRenderingContext);
    GLenum pname = duk_get_uint(ctx, 0);
    GLint  param = duk_is_boolean(ctx, 1) ? duk_get_boolean(ctx, 1) : duk_get_int(ctx, 1);
    switch (pname) {
        case GL_UNPACK_FLIP_Y_WEBGL:
            thiz->unpackFlipY = param;
            break;
        case GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL:
            thiz->unpackPremultiplyAlpha = param;
            break;
        case GL_UNPACK_COLORSPACE_CONVERSION_WEBGL:
            if (param == GL_BROWSER_DEFAULT_WEBGL || param == GL_NONE) {
                thiz->unpackColorspaceConversion = param;
            }
            break;
        case GL_PACK_ALIGNMENT:
            thiz->packAlignment = param;
            glPixelStorei(pname, param);
            break;
        case GL_UNPACK_ALIGNMENT:
            thiz->unpackAlignment = param;
            glPixelStorei(pname, param);
            break;
        default:
            glPixelStorei(pname, param);
            break;
    }
    return 0;
}
int AEWebGLRenderingContext::polygonOffset(duk_context* ctx) {
    GLfloat factor = duk_get_number(ctx, 0);
    GLfloat units  = duk_get_number(ctx, 1);
    glPolygonOffset(factor, units);
    return 0;
}
int AEWebGLRenderingContext::readPixels(duk_context* ctx) {
    size_t  size = 0;
    GLint   x    = duk_get_int(ctx, 0);
    GLint   y    = duk_get_int(ctx, 1);
    GLint   w    = duk_get_int(ctx, 2);
    GLint   h    = duk_get_int(ctx, 3);
    GLuint  fmt  = duk_get_uint(ctx, 4);
    GLuint  type = duk_get_uint(ctx, 5);
    GLvoid* data = duk_get_buffer_data(ctx, 6, &size);
    glReadPixels(x, y, w, h, fmt, type, data);
    return 0;
}
int AEWebGLRenderingContext::releaseShaderCompiler(duk_context* ctx) {
    glReleaseShaderCompiler();
    return 0;
}
int AEWebGLRenderingContext::renderbufferStorage(duk_context* ctx) {
    GLenum  target = duk_get_uint(ctx, 0);
    GLenum  format = duk_get_uint(ctx, 1);
    GLsizei width  = duk_get_int(ctx, 2);
    GLsizei height = duk_get_int(ctx, 3);
    if (format == GL_DEPTH_STENCIL) {
        glRenderbufferStorage(target, GL_DEPTH24_STENCIL8_OES, width, height);
    }
    else {
        glRenderbufferStorage(target, format, width, height);
    }
    return 0;
}
int AEWebGLRenderingContext::sampleCoverage(duk_context* ctx) {
    GLclampf  value  = duk_get_number(ctx, 0);
    GLboolean invert = duk_get_boolean(ctx, 1);
    glSampleCoverage(value, invert);
    return 0;
}
int AEWebGLRenderingContext::scissor(duk_context* ctx) {
    GLint   x = duk_get_int(ctx, 0);
    GLint   y = duk_get_int(ctx, 1);
    GLsizei w = duk_get_int(ctx, 2);
    GLsizei h = duk_get_int(ctx, 3);
    glScissor(x, y, w, h);
    return 0;
}
int AEWebGLRenderingContext::shaderBinary(duk_context* ctx) {
    LOGW("%s", "WebGL not support.");
    return 0;
}
int AEWebGLRenderingContext::shaderSource(duk_context* ctx) {
    const GLuint  shader = duk_get_uint(ctx, 0);
    const GLchar* source = duk_get_string(ctx, 1);
    const GLsizei length = (GLsizei)strlen(source);
    glShaderSource(shader, 1, &source, &length);
    return 0;
}
int AEWebGLRenderingContext::stencilFunc(duk_context* ctx) {
    GLenum func = duk_get_uint(ctx, 0);
    GLint  ref  = duk_get_int(ctx, 1);
    GLuint mask = duk_get_uint(ctx, 2);
    glStencilFunc(func, ref, mask);
    return 0;
}
int AEWebGLRenderingContext::stencilFuncSeparate(duk_context* ctx) {
    GLenum face = duk_get_uint(ctx, 0);
    GLenum func = duk_get_uint(ctx, 1);
    GLint  ref  = duk_get_int(ctx, 2);
    GLuint mask = duk_get_uint(ctx, 3);
    glStencilFuncSeparate(face, func, ref, mask);
    return 0;
}
int AEWebGLRenderingContext::stencilMask(duk_context* ctx) {
    GLuint mask = duk_get_uint(ctx, 0);
    glStencilMask(mask);
    return 0;
}
int AEWebGLRenderingContext::stencilMaskSeparate(duk_context* ctx) {
    GLenum face = duk_get_uint(ctx, 0);
    GLuint mask = duk_get_uint(ctx, 1);
    glStencilMaskSeparate(face, mask);
    return 0;
}
int AEWebGLRenderingContext::stencilOp(duk_context* ctx) {
    GLenum fail  = duk_get_uint(ctx, 0);
    GLenum zfail = duk_get_uint(ctx, 1);
    GLenum zpass = duk_get_uint(ctx, 2);
    glStencilOp(fail, zfail, zpass);
    return 0;
}
int AEWebGLRenderingContext::stencilOpSeparate(duk_context* ctx) {
    GLenum face  = duk_get_uint(ctx, 0);
    GLenum fail  = duk_get_uint(ctx, 1);
    GLenum zfail = duk_get_uint(ctx, 2);
    GLenum zpass = duk_get_uint(ctx, 3);
    glStencilOpSeparate(face, fail, zfail, zpass);
    return 0;
}
int AEWebGLRenderingContext::texImage2D(duk_context* ctx) {
    js_thiz(AEWebGLRenderingContext);
    uint argc  = duk_get_top(ctx);
    bool flipY = thiz->unpackFlipY;
    bool alpha = thiz->unpackPremultiplyAlpha;

    if (argc == 9) {
    	GLenum  target = duk_get_uint(ctx, 0);
    	GLint   level  = duk_get_int( ctx, 1);
    	GLint   infmt  = duk_get_int( ctx, 2);
    	GLint   width  = duk_get_int( ctx, 3);
    	GLint   height = duk_get_int( ctx, 4);
    	GLint   border = duk_get_int( ctx, 5);
    	GLenum  format = duk_get_uint(ctx, 6);
    	GLenum  type   = duk_get_uint(ctx, 7);
        GLvoid* pixels = duk_get_buffer_data(ctx, 8, nullptr);
        
        AEFormat::convert(pixels, width, height, flipY, alpha);
        glTexImage2D(target, level, infmt, width, height, border, format, type, pixels);
    }
    else if (argc == 6) {
    	GLenum target = duk_get_uint(ctx, 0);
    	GLint  level  = duk_get_int( ctx, 1);
    	GLint  infmt  = duk_get_int( ctx, 2);
    	GLenum format = duk_get_uint(ctx, 3);
    	GLenum type   = duk_get_uint(ctx, 4);
        
    	infmt  = GL_RGBA; // texture.format = isJPEG ? RGBFormat : RGBAFormat;
    	format = GL_RGBA; // !TODO: three.js这样写，我也很为难呀，只好写死了RGBA格式;
        js_that(-1, AEImage, node);
        if (node) {
        	node->texImage2D(target, level, infmt, format, type, flipY, alpha);
        }
        else {
        	duk_get_prop_string(ctx, -1, "_context");
            js_that(-1, AEImage, canvas);
			duk_pop(ctx);
			canvas->texImage2D(target, level, infmt, format, type, flipY, alpha);
        }
    }
    else {
        LOG("texImage2D: nargs error:[%d]", argc);
    }
    return 0;
}
int AEWebGLRenderingContext::texParameterf(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLfloat param = duk_get_number(ctx, 2);
    glTexParameterf(target, pname, param);
    return 0;
}
int AEWebGLRenderingContext::texParameterfv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLfloat param = 0;
    glTexParameterfv(target, pname, &param);
    duk_push_number(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::texParameteri(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLint   param = duk_get_int(ctx, 2);
    glTexParameteri(target, pname, param);
    return 0;
}
int AEWebGLRenderingContext::texParameteriv(duk_context* ctx) {
    GLenum target = duk_get_uint(ctx, 0);
    GLenum  pname = duk_get_uint(ctx, 1);
    GLint   param = 0;
    glTexParameteriv(target, pname, &param);
    duk_push_int(ctx, param);
    return 1;
}
int AEWebGLRenderingContext::texSubImage2D(duk_context* ctx) {
    js_thiz(AEWebGLRenderingContext);
    uint argc  = duk_get_top(ctx);
    bool flipY = thiz->unpackFlipY;
    bool alpha = thiz->unpackPremultiplyAlpha;

    if (argc == 9) {
        GLenum  target = duk_get_uint(ctx, 0);
        GLint    level = duk_get_int(ctx, 1);
        GLint  xoffset = duk_get_int(ctx, 2);
        GLint  yoffset = duk_get_int(ctx, 3);
        GLsizei  width = duk_get_int(ctx, 4);
        GLsizei height = duk_get_int(ctx, 5);
        GLenum  format = duk_get_uint(ctx, 6);
        GLenum    type = duk_get_uint(ctx, 7);
        GLvoid* pixels = duk_get_buffer_data(ctx, 8, nullptr);

        AEFormat::convert(pixels, width, height, flipY, alpha);
        glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
    }
    else if (argc == 7) {
        GLenum  target = duk_get_uint(ctx, 0);
        GLint    level = duk_get_int(ctx, 1);
        GLint  xoffset = duk_get_int(ctx, 2);
        GLint  yoffset = duk_get_int(ctx, 3);
        GLenum  format = duk_get_uint(ctx, 4);
        GLenum    type = duk_get_uint(ctx, 5);
        
        format = GL_RGBA; // !TODO: three.js这样写，我也很为难呀，只好写死了RGBA格式;
        js_that(-1, AEImage, node);
        if (node) {
        	node->texSubImage2D(target, level, xoffset, yoffset, format, type, flipY, alpha);
        }
        else {
            duk_get_prop_string(ctx, -1, "_context");
            js_that(-1, AEImage, canvas);
            duk_pop(ctx);
            canvas->texSubImage2D(target, level, xoffset, yoffset, format, type, flipY, alpha);
        }
    }
    else {
        LOG("texSubImage2D: argc is %d.", argc);
    }
    return 0;
}
int AEWebGLRenderingContext::uniform1f(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLfloat x = duk_get_number(ctx, 1);
    glUniform1f(idx, x);
    return 0;
}
int AEWebGLRenderingContext::uniform1fv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 1, [&](GLint count, GLfloat* value) {
        glUniform1fv(location, count, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform1i(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLint   x = duk_get_int(ctx, 1);
    glUniform1i(idx, x);
    return 0;
}
int AEWebGLRenderingContext::uniform1iv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLint>(ctx, 1, [&](GLint count, GLint* value) {
        glUniform1iv(location, count, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform2f(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLfloat x = duk_get_number(ctx, 1);
    GLfloat y = duk_get_number(ctx, 2);
    glUniform2f(idx, x, y);
    return 0;
}
int AEWebGLRenderingContext::uniform2fv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 1, [&](GLint count, GLfloat* value) {
        glUniform2fv(location, count / 2, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform2i(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLint   x = duk_get_int(ctx, 1);
    GLint   y = duk_get_int(ctx, 2);
    glUniform2i(idx, x, y);
    return 0;
}
int AEWebGLRenderingContext::uniform2iv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLint>(ctx, 1, [&](GLint count, GLint* value) {
        glUniform2iv(location, count / 2, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform3f(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLfloat x = duk_get_number(ctx, 1);
    GLfloat y = duk_get_number(ctx, 2);
    GLfloat z = duk_get_number(ctx, 3);
    glUniform3f(idx, x, y, z);
    return 0;
}
int AEWebGLRenderingContext::uniform3fv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 1, [&](GLint count, GLfloat* value) {
        glUniform3fv(location, count / 3, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform3i(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLint   x = duk_get_int(ctx, 1);
    GLint   y = duk_get_int(ctx, 2);
    GLint   z = duk_get_int(ctx, 3);
    glUniform3i(idx, x, y, z);
    return 0;
}
int AEWebGLRenderingContext::uniform3iv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLint>(ctx, 1, [&](GLint count, GLint* value) {
        glUniform3iv(location, count / 3, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform4f(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLfloat x = duk_get_number(ctx, 1);
    GLfloat y = duk_get_number(ctx, 2);
    GLfloat z = duk_get_number(ctx, 3);
    GLfloat w = duk_get_number(ctx, 4);
    glUniform4f(idx, x, y, z, w);
    return 0;
}
int AEWebGLRenderingContext::uniform4fv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 1, [&](GLint count, GLfloat* value) {
        glUniform4fv(location, count / 4, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniform4i(duk_context* ctx) {
    GLint idx = duk_get_int(ctx, 0);
    GLint   x = duk_get_int(ctx, 1);
    GLint   y = duk_get_int(ctx, 2);
    GLint   z = duk_get_int(ctx, 3);
    GLint   w = duk_get_int(ctx, 4);
    glUniform4i(idx, x, y, z, w);
    return 0;
}
int AEWebGLRenderingContext::uniform4iv(duk_context* ctx) {
    GLint location = duk_get_int(ctx, 0);
    AEFormat::getTypeBuffer<GLint>(ctx, 1, [&](GLint count, GLint* value) {
        glUniform4iv(location, count / 4, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniformMatrix2fv(duk_context* ctx) {
    GLint   location  = duk_get_int(ctx, 0);
    boolean transpose = duk_get_boolean(ctx, 1);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 2, [&](GLuint count, GLfloat* value) {
        glUniformMatrix2fv(location, count / 4, transpose, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniformMatrix3fv(duk_context* ctx) {
    GLint   location  = duk_get_int(ctx, 0);
    boolean transpose = duk_get_boolean(ctx, 1);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 2, [&](GLuint count, GLfloat* value) {
        glUniformMatrix3fv(location, count / 9, transpose, value);
    });
    return 0;
}
int AEWebGLRenderingContext::uniformMatrix4fv(duk_context* ctx) {
    GLint   location  = duk_get_int(ctx, 0);
    boolean transpose = duk_get_boolean(ctx, 1);
    AEFormat::getTypeBuffer<GLfloat>(ctx, 2, [&](GLuint count, GLfloat* value) {
        glUniformMatrix4fv(location, count / 16, transpose, value);
    });
    return 0;
}
int AEWebGLRenderingContext::useProgram(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    glUseProgram(program);
    return 0;
}
int AEWebGLRenderingContext::validateProgram(duk_context* ctx) {
    GLuint program = duk_get_uint(ctx, 0);
    glValidateProgram(program);
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib1f(duk_context* ctx) {
    GLuint idx = duk_get_uint(ctx, 0);
    GLfloat  x = duk_get_number(ctx, 1);
    glVertexAttrib1f(idx, x);
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib1fv(duk_context* ctx) {
    GLuint  index  = duk_get_uint(ctx, 0);
    GLvoid* values = duk_get_buffer_data(ctx, -1, nullptr);
    glVertexAttrib1fv(index, static_cast<GLfloat*>(values));
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib2f(duk_context* ctx) {
    GLuint idx = duk_get_uint(ctx, 0);
    GLfloat  x = duk_get_number(ctx, 1);
    GLfloat  y = duk_get_number(ctx, 2);
    glVertexAttrib2f(idx, x, y);
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib2fv(duk_context* ctx) {
    GLuint  index  = duk_get_uint(ctx, 0);
    GLvoid* values = duk_get_buffer_data(ctx, -1, nullptr);
    glVertexAttrib2fv(index, static_cast<GLfloat*>(values));
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib3f(duk_context* ctx) {
    GLuint idx = duk_get_uint(ctx, 0);
    GLfloat  x = duk_get_number(ctx, 1);
    GLfloat  y = duk_get_number(ctx, 2);
    GLfloat  z = duk_get_number(ctx, 3);
    glVertexAttrib3f(idx, x, y, z);
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib3fv(duk_context* ctx) {
    GLuint  index  = duk_get_uint(ctx, 0);
    GLvoid* values = duk_get_buffer_data(ctx, -1, nullptr);
    glVertexAttrib3fv(index, static_cast<GLfloat*>(values));
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib4f(duk_context* ctx) {
    GLuint idx = duk_get_uint(ctx, 0);
    GLfloat  x = duk_get_number(ctx, 1);
    GLfloat  y = duk_get_number(ctx, 2);
    GLfloat  z = duk_get_number(ctx, 3);
    GLfloat  w = duk_get_number(ctx, 4);
    glVertexAttrib4f(idx, x, y, z, w);
    return 0;
}
int AEWebGLRenderingContext::vertexAttrib4fv(duk_context* ctx) {
    GLuint  index  = duk_get_uint(ctx, 0);
    GLvoid* values = duk_get_buffer_data(ctx, -1, nullptr);
    glVertexAttrib4fv(index, static_cast<GLfloat*>(values));
    return 0;
}
int AEWebGLRenderingContext::vertexAttribPointer(duk_context* ctx) {
    GLuint  indx   = duk_get_uint(ctx, 0);
    GLint   size   = duk_get_int(ctx, 1);
    GLenum  type   = duk_get_uint(ctx, 2);
    boolean normal = duk_get_boolean(ctx, 3);
    GLsizei stride = duk_get_int(ctx, 4);
    GLint   offset = duk_get_int(ctx, 5);
    glVertexAttribPointer(indx, size, type, normal, stride, reinterpret_cast<GLvoid*>(offset));
    return 0;
}
int AEWebGLRenderingContext::viewport(duk_context* ctx) {
    GLint   x = duk_get_int(ctx, 0);
    GLint   y = duk_get_int(ctx, 1);
    GLsizei w = duk_get_int(ctx, 2);
    GLsizei h = duk_get_int(ctx, 3);
    glViewport(x, y, w, h);
    return 0;
}

void AEWebGLRenderingContext::binding(duk_context *ctx) {
    js_begin_binding();
    
    js_binding_func(2, size);
    js_add_prop(alpha);
    js_add_prop(antialias);
    js_add_prop(depth);
    js_add_prop(stencil);
    js_add_prop(packAlignment);
    js_add_prop(premultipliedAlpha);
    js_add_prop(preserveDrawingBuffer);
    js_add_prop(unpackFlipY);
    js_add_prop(unpackAlignment);
    js_add_prop(unpackPremultiplyAlpha);
    js_add_prop(unpackColorspaceConversion);
    
    js_binding_func(1, clearDepth);
    js_binding_func(0, createBuffer);
    js_binding_func(0, createFramebuffer);
    js_binding_func(0, createRenderbuffer);
    js_binding_func(0, createTexture);
    js_binding_func(1, deleteBuffer);
    js_binding_func(1, deleteFramebuffer);
    js_binding_func(1, deleteRenderbuffer);
    js_binding_func(1, deleteTexture);
    js_binding_func(2, depthRange);
    js_binding_func(2, getBufferParameter);
    js_binding_func(0, getContextAttributes);
    js_binding_func(1, getExtension);
    js_binding_func(3, getFramebufferAttachmentParameter);
    js_binding_func(1, getParameter);
    js_binding_func(2, getProgramParameter);
    js_binding_func(2, getRenderbufferParameter);
    js_binding_func(2, getShaderParameter);
    js_binding_func(0, getSupportedExtensions);
    js_binding_func(2, getTexParameter);
    js_binding_func(2, getUniform);
    js_binding_func(2, getVertexAttrib);
    js_binding_func(2, getVertexAttribOffset);
    js_binding_func(0, isContextLost);
    
    js_binding_func(1, activeTexture);
    js_binding_func(2, attachShader);
    js_binding_func(3, bindAttribLocation);
    js_binding_func(2, bindBuffer);
    js_binding_func(2, bindFramebuffer);
    js_binding_func(2, bindRenderbuffer);
    js_binding_func(2, bindTexture);
    js_binding_func(4, blendColor);
    js_binding_func(1, blendEquation);
    js_binding_func(2, blendEquationSeparate);
    js_binding_func(2, blendFunc);
    js_binding_func(4, blendFuncSeparate);
    js_binding_func(3, bufferData);
    js_binding_func(3, bufferSubData);
    js_binding_func(1, checkFramebufferStatus);
    js_binding_func(1, clear);
    js_binding_func(4, clearColor);
    js_binding_func(1, clearDepthf);
    js_binding_func(1, clearStencil);
    js_binding_func(4, colorMask);
    js_binding_func(1, compileShader);
    js_binding_func(7, compressedTexImage2D);
    js_binding_func(8, compressedTexSubImage2D);
    js_binding_func(8, copyTexImage2D);
    js_binding_func(8, copyTexSubImage2D);
    js_binding_func(0, createProgram);
    js_binding_func(1, createShader);
    js_binding_func(1, cullFace);
    js_binding_func(1, deleteBuffers);
    js_binding_func(1, deleteFramebuffers);
    js_binding_func(1, deleteProgram);
    js_binding_func(1, deleteRenderbuffers);
    js_binding_func(1, deleteShader);
    js_binding_func(1, deleteTextures);
    js_binding_func(1, depthFunc);
    js_binding_func(1, depthMask);
    js_binding_func(2, depthRangef);
    js_binding_func(2, detachShader);
    js_binding_func(1, disable);
    js_binding_func(1, disableVertexAttribArray);
    js_binding_func(3, drawArrays);
    js_binding_func(4, drawElements);
    js_binding_func(1, enable);
    js_binding_func(1, enableVertexAttribArray);
    js_binding_func(0, finish);
    js_binding_func(0, flush);
    js_binding_func(4, framebufferRenderbuffer);
    js_binding_func(5, framebufferTexture2D);
    js_binding_func(1, frontFace);
    js_binding_func(1, genBuffers);
    js_binding_func(1, generateMipmap);
    js_binding_func(1, genFramebuffers);
    js_binding_func(1, genRenderbuffers);
    js_binding_func(1, genTextures);
    js_binding_func(2, getActiveAttrib);
    js_binding_func(2, getActiveUniform);
    js_binding_func(1, getAttachedShaders);
    js_binding_func(2, getAttribLocation);
    js_binding_func(1, getBooleanv);
    js_binding_func(2, getBufferParameteriv);
    js_binding_func(0, getError);
    js_binding_func(1, getFloatv);
    js_binding_func(3, getFramebufferAttachmentParameteriv);
    js_binding_func(1, getIntegerv);
    js_binding_func(2, getProgramiv);
    js_binding_func(1, getProgramInfoLog);
    js_binding_func(2, getRenderbufferParameteriv);
    js_binding_func(2, getShaderiv);
    js_binding_func(1, getShaderInfoLog);
    js_binding_func(2, getShaderPrecisionFormat);
    js_binding_func(1, getShaderSource);
    js_binding_func(1, getString);
    js_binding_func(2, getTexParameterfv);
    js_binding_func(2, getTexParameteriv);
    js_binding_func(2, getUniformfv);
    js_binding_func(2, getUniformiv);
    js_binding_func(2, getUniformLocation);
    js_binding_func(2, getVertexAttribfv);
    js_binding_func(2, getVertexAttribiv);
    js_binding_func(2, getVertexAttribPointerv);
    js_binding_func(2, hint);
    js_binding_func(1, isBuffer);
    js_binding_func(1, isEnabled);
    js_binding_func(1, isFramebuffer);
    js_binding_func(1, isProgram);
    js_binding_func(1, isRenderbuffer);
    js_binding_func(1, isShader);
    js_binding_func(1, isTexture);
    js_binding_func(1, lineWidth);
    js_binding_func(1, linkProgram);
    js_binding_func(2, pixelStorei);
    js_binding_func(2, polygonOffset);
    js_binding_func(7, readPixels);
    js_binding_func(0, releaseShaderCompiler);
    js_binding_func(4, renderbufferStorage);
    js_binding_func(2, sampleCoverage);
    js_binding_func(4, scissor);
    js_binding_func(1, shaderBinary);
    js_binding_func(2, shaderSource);
    js_binding_func(3, stencilFunc);
    js_binding_func(4, stencilFuncSeparate);
    js_binding_func(1, stencilMask);
    js_binding_func(2, stencilMaskSeparate);
    js_binding_func(3, stencilOp);
    js_binding_func(4, stencilOpSeparate);
    js_binding_func(DUK_VARARGS, texImage2D);
    js_binding_func(3, texParameterf);
    js_binding_func(2, texParameterfv);
    js_binding_func(3, texParameteri);
    js_binding_func(2, texParameteriv);
    js_binding_func(DUK_VARARGS, texSubImage2D);
    js_binding_func(2, uniform1f);
    js_binding_func(2, uniform1fv);
    js_binding_func(2, uniform1i);
    js_binding_func(2, uniform1iv);
    js_binding_func(3, uniform2f);
    js_binding_func(2, uniform2fv);
    js_binding_func(3, uniform2i);
    js_binding_func(2, uniform2iv);
    js_binding_func(4, uniform3f);
    js_binding_func(2, uniform3fv);
    js_binding_func(4, uniform3i);
    js_binding_func(2, uniform3iv);
    js_binding_func(5, uniform4f);
    js_binding_func(2, uniform4fv);
    js_binding_func(5, uniform4i);
    js_binding_func(2, uniform4iv);
    js_binding_func(3, uniformMatrix2fv);
    js_binding_func(3, uniformMatrix3fv);
    js_binding_func(3, uniformMatrix4fv);
    js_binding_func(1, useProgram);
    js_binding_func(1, validateProgram);
    js_binding_func(2, vertexAttrib1f);
    js_binding_func(2, vertexAttrib1fv);
    js_binding_func(3, vertexAttrib2f);
    js_binding_func(2, vertexAttrib2fv);
    js_binding_func(4, vertexAttrib3f);
    js_binding_func(2, vertexAttrib3fv);
    js_binding_func(5, vertexAttrib4f);
    js_binding_func(2, vertexAttrib4fv);
    js_binding_func(6, vertexAttribPointer);
    js_binding_func(4, viewport);
    
    ctx_set_gl(DEPTH_BUFFER_BIT);
    ctx_set_gl(STENCIL_BUFFER_BIT);
    ctx_set_gl(COLOR_BUFFER_BIT);
    ctx_set_gl(FALSE);
    ctx_set_gl(TRUE);
    ctx_set_gl(POINTS);
    ctx_set_gl(LINES);
    ctx_set_gl(LINE_LOOP);
    ctx_set_gl(LINE_STRIP);
    ctx_set_gl(TRIANGLES);
    ctx_set_gl(TRIANGLE_STRIP);
    ctx_set_gl(TRIANGLE_FAN);
    ctx_set_gl(ZERO);
    ctx_set_gl(ONE);
    ctx_set_gl(SRC_COLOR);
    ctx_set_gl(ONE_MINUS_SRC_COLOR);
    ctx_set_gl(SRC_ALPHA);
    ctx_set_gl(ONE_MINUS_SRC_ALPHA);
    ctx_set_gl(DST_ALPHA);
    ctx_set_gl(ONE_MINUS_DST_ALPHA);
    ctx_set_gl(DST_COLOR);
    ctx_set_gl(ONE_MINUS_DST_COLOR);
    ctx_set_gl(SRC_ALPHA_SATURATE);
    ctx_set_gl(FUNC_ADD);
    ctx_set_gl(BLEND_EQUATION);
    ctx_set_gl(BLEND_EQUATION_RGB);
    ctx_set_gl(BLEND_EQUATION_ALPHA);
    ctx_set_gl(FUNC_SUBTRACT);
    ctx_set_gl(FUNC_REVERSE_SUBTRACT);
    ctx_set_gl(BLEND_DST_RGB);
    ctx_set_gl(BLEND_SRC_RGB);
    ctx_set_gl(BLEND_DST_ALPHA);
    ctx_set_gl(BLEND_SRC_ALPHA);
    ctx_set_gl(CONSTANT_COLOR);
    ctx_set_gl(ONE_MINUS_CONSTANT_COLOR);
    ctx_set_gl(CONSTANT_ALPHA);
    ctx_set_gl(ONE_MINUS_CONSTANT_ALPHA);
    ctx_set_gl(BLEND_COLOR);
    ctx_set_gl(ARRAY_BUFFER);
    ctx_set_gl(ELEMENT_ARRAY_BUFFER);
    ctx_set_gl(ARRAY_BUFFER_BINDING);
    ctx_set_gl(ELEMENT_ARRAY_BUFFER_BINDING);
    ctx_set_gl(STREAM_DRAW);
    ctx_set_gl(STATIC_DRAW);
    ctx_set_gl(DYNAMIC_DRAW);
    ctx_set_gl(BUFFER_SIZE);
    ctx_set_gl(BUFFER_USAGE);
    ctx_set_gl(CURRENT_VERTEX_ATTRIB);
    ctx_set_gl(FRONT);
    ctx_set_gl(BACK);
    ctx_set_gl(FRONT_AND_BACK);
    ctx_set_gl(TEXTURE_2D);
    ctx_set_gl(CULL_FACE);
    ctx_set_gl(BLEND);
    ctx_set_gl(DITHER);
    ctx_set_gl(STENCIL_TEST);
    ctx_set_gl(DEPTH_TEST);
    ctx_set_gl(SCISSOR_TEST);
    ctx_set_gl(POLYGON_OFFSET_FILL);
    ctx_set_gl(SAMPLE_ALPHA_TO_COVERAGE);
    ctx_set_gl(SAMPLE_COVERAGE);
    ctx_set_gl(NO_ERROR);
    ctx_set_gl(INVALID_ENUM);
    ctx_set_gl(INVALID_VALUE);
    ctx_set_gl(INVALID_OPERATION);
    ctx_set_gl(OUT_OF_MEMORY);
    ctx_set_gl(CW);
    ctx_set_gl(CCW);
    ctx_set_gl(LINE_WIDTH);
    ctx_set_gl(ALIASED_POINT_SIZE_RANGE);
    ctx_set_gl(ALIASED_LINE_WIDTH_RANGE);
    ctx_set_gl(CULL_FACE_MODE);
    ctx_set_gl(FRONT_FACE);
    ctx_set_gl(DEPTH_RANGE);
    ctx_set_gl(DEPTH_WRITEMASK);
    ctx_set_gl(DEPTH_CLEAR_VALUE);
    ctx_set_gl(DEPTH_FUNC);
    ctx_set_gl(STENCIL_CLEAR_VALUE);
    ctx_set_gl(STENCIL_FUNC);
    ctx_set_gl(STENCIL_FAIL);
    ctx_set_gl(STENCIL_PASS_DEPTH_FAIL);
    ctx_set_gl(STENCIL_PASS_DEPTH_PASS);
    ctx_set_gl(STENCIL_REF);
    ctx_set_gl(STENCIL_VALUE_MASK);
    ctx_set_gl(STENCIL_WRITEMASK);
    ctx_set_gl(STENCIL_BACK_FUNC);
    ctx_set_gl(STENCIL_BACK_FAIL);
    ctx_set_gl(STENCIL_BACK_PASS_DEPTH_FAIL);
    ctx_set_gl(STENCIL_BACK_PASS_DEPTH_PASS);
    ctx_set_gl(STENCIL_BACK_REF);
    ctx_set_gl(STENCIL_BACK_VALUE_MASK);
    ctx_set_gl(STENCIL_BACK_WRITEMASK);
    ctx_set_gl(VIEWPORT);
    ctx_set_gl(SCISSOR_BOX);
    ctx_set_gl(COLOR_CLEAR_VALUE);
    ctx_set_gl(COLOR_WRITEMASK);
    ctx_set_gl(UNPACK_ALIGNMENT);
    ctx_set_gl(PACK_ALIGNMENT);
    ctx_set_gl(MAX_TEXTURE_SIZE);
    ctx_set_gl(MAX_VIEWPORT_DIMS);
    ctx_set_gl(SUBPIXEL_BITS);
    ctx_set_gl(RED_BITS);
    ctx_set_gl(GREEN_BITS);
    ctx_set_gl(BLUE_BITS);
    ctx_set_gl(ALPHA_BITS);
    ctx_set_gl(DEPTH_BITS);
    ctx_set_gl(STENCIL_BITS);
    ctx_set_gl(POLYGON_OFFSET_UNITS);
    ctx_set_gl(POLYGON_OFFSET_FACTOR);
    ctx_set_gl(TEXTURE_BINDING_2D);
    ctx_set_gl(SAMPLE_BUFFERS);
    ctx_set_gl(SAMPLES);
    ctx_set_gl(SAMPLE_COVERAGE_VALUE);
    ctx_set_gl(SAMPLE_COVERAGE_INVERT);
    ctx_set_gl(NUM_COMPRESSED_TEXTURE_FORMATS);
    ctx_set_gl(COMPRESSED_TEXTURE_FORMATS);
    ctx_set_gl(DONT_CARE);
    ctx_set_gl(FASTEST);
    ctx_set_gl(NICEST);
    ctx_set_gl(GENERATE_MIPMAP_HINT);
    ctx_set_gl(BYTE);
    ctx_set_gl(UNSIGNED_BYTE);
    ctx_set_gl(SHORT);
    ctx_set_gl(UNSIGNED_SHORT);
    ctx_set_gl(INT);
    ctx_set_gl(UNSIGNED_INT);
    ctx_set_gl(FLOAT);
    ctx_set_gl(FIXED);
    ctx_set_gl(DEPTH_COMPONENT);
    ctx_set_gl(ALPHA);
    ctx_set_gl(RGB);
    ctx_set_gl(RGBA);
    ctx_set_gl(LUMINANCE);
    ctx_set_gl(LUMINANCE_ALPHA);
    ctx_set_gl(UNSIGNED_SHORT_4_4_4_4);
    ctx_set_gl(UNSIGNED_SHORT_5_5_5_1);
    ctx_set_gl(UNSIGNED_SHORT_5_6_5);
    ctx_set_gl(FRAGMENT_SHADER);
    ctx_set_gl(VERTEX_SHADER);
    ctx_set_gl(MAX_VERTEX_ATTRIBS);
    ctx_set_gl(MAX_VERTEX_UNIFORM_VECTORS);
    ctx_set_gl(MAX_VARYING_VECTORS);
    ctx_set_gl(MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    ctx_set_gl(MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    ctx_set_gl(MAX_TEXTURE_IMAGE_UNITS);
    ctx_set_gl(MAX_FRAGMENT_UNIFORM_VECTORS);
    ctx_set_gl(SHADER_TYPE);
    ctx_set_gl(DELETE_STATUS);
    ctx_set_gl(LINK_STATUS);
    ctx_set_gl(VALIDATE_STATUS);
    ctx_set_gl(ATTACHED_SHADERS);
    ctx_set_gl(ACTIVE_UNIFORMS);
    ctx_set_gl(ACTIVE_UNIFORM_MAX_LENGTH);
    ctx_set_gl(ACTIVE_ATTRIBUTES);
    ctx_set_gl(ACTIVE_ATTRIBUTE_MAX_LENGTH);
    ctx_set_gl(SHADING_LANGUAGE_VERSION);
    ctx_set_gl(CURRENT_PROGRAM);
    ctx_set_gl(NEVER);
    ctx_set_gl(LESS);
    ctx_set_gl(EQUAL);
    ctx_set_gl(LEQUAL);
    ctx_set_gl(GREATER);
    ctx_set_gl(NOTEQUAL);
    ctx_set_gl(GEQUAL);
    ctx_set_gl(ALWAYS);
    ctx_set_gl(KEEP);
    ctx_set_gl(REPLACE);
    ctx_set_gl(INCR);
    ctx_set_gl(DECR);
    ctx_set_gl(INVERT);
    ctx_set_gl(INCR_WRAP);
    ctx_set_gl(DECR_WRAP);
    ctx_set_gl(VENDOR);
    ctx_set_gl(RENDERER);
    ctx_set_gl(VERSION);
    ctx_set_gl(EXTENSIONS);
    ctx_set_gl(NEAREST);
    ctx_set_gl(LINEAR);
    ctx_set_gl(NEAREST_MIPMAP_NEAREST);
    ctx_set_gl(LINEAR_MIPMAP_NEAREST);
    ctx_set_gl(NEAREST_MIPMAP_LINEAR);
    ctx_set_gl(LINEAR_MIPMAP_LINEAR);
    ctx_set_gl(TEXTURE_MAG_FILTER);
    ctx_set_gl(TEXTURE_MIN_FILTER);
    ctx_set_gl(TEXTURE_WRAP_S);
    ctx_set_gl(TEXTURE_WRAP_T);
    ctx_set_gl(TEXTURE);
    ctx_set_gl(TEXTURE_CUBE_MAP);
    ctx_set_gl(TEXTURE_BINDING_CUBE_MAP);
    ctx_set_gl(TEXTURE_CUBE_MAP_POSITIVE_X);
    ctx_set_gl(TEXTURE_CUBE_MAP_NEGATIVE_X);
    ctx_set_gl(TEXTURE_CUBE_MAP_POSITIVE_Y);
    ctx_set_gl(TEXTURE_CUBE_MAP_NEGATIVE_Y);
    ctx_set_gl(TEXTURE_CUBE_MAP_POSITIVE_Z);
    ctx_set_gl(TEXTURE_CUBE_MAP_NEGATIVE_Z);
    ctx_set_gl(MAX_CUBE_MAP_TEXTURE_SIZE);
    ctx_set_gl(TEXTURE0);
    ctx_set_gl(TEXTURE1);
    ctx_set_gl(TEXTURE2);
    ctx_set_gl(TEXTURE3);
    ctx_set_gl(TEXTURE4);
    ctx_set_gl(TEXTURE5);
    ctx_set_gl(TEXTURE6);
    ctx_set_gl(TEXTURE7);
    ctx_set_gl(TEXTURE8);
    ctx_set_gl(TEXTURE9);
    ctx_set_gl(TEXTURE10);
    ctx_set_gl(TEXTURE11);
    ctx_set_gl(TEXTURE12);
    ctx_set_gl(TEXTURE13);
    ctx_set_gl(TEXTURE14);
    ctx_set_gl(TEXTURE15);
    ctx_set_gl(TEXTURE16);
    ctx_set_gl(TEXTURE17);
    ctx_set_gl(TEXTURE18);
    ctx_set_gl(TEXTURE19);
    ctx_set_gl(TEXTURE20);
    ctx_set_gl(TEXTURE21);
    ctx_set_gl(TEXTURE22);
    ctx_set_gl(TEXTURE23);
    ctx_set_gl(TEXTURE24);
    ctx_set_gl(TEXTURE25);
    ctx_set_gl(TEXTURE26);
    ctx_set_gl(TEXTURE27);
    ctx_set_gl(TEXTURE28);
    ctx_set_gl(TEXTURE29);
    ctx_set_gl(TEXTURE30);
    ctx_set_gl(TEXTURE31);
    ctx_set_gl(ACTIVE_TEXTURE);
    ctx_set_gl(REPEAT);
    ctx_set_gl(CLAMP_TO_EDGE);
    ctx_set_gl(MIRRORED_REPEAT);
    ctx_set_gl(FLOAT_VEC2);
    ctx_set_gl(FLOAT_VEC3);
    ctx_set_gl(FLOAT_VEC4);
    ctx_set_gl(INT_VEC2);
    ctx_set_gl(INT_VEC3);
    ctx_set_gl(INT_VEC4);
    ctx_set_gl(BOOL);
    ctx_set_gl(BOOL_VEC2);
    ctx_set_gl(BOOL_VEC3);
    ctx_set_gl(BOOL_VEC4);
    ctx_set_gl(FLOAT_MAT2);
    ctx_set_gl(FLOAT_MAT3);
    ctx_set_gl(FLOAT_MAT4);
    ctx_set_gl(SAMPLER_2D);
    ctx_set_gl(SAMPLER_CUBE);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_ENABLED);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_SIZE);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_STRIDE);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_TYPE);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_NORMALIZED);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_POINTER);
    ctx_set_gl(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
    ctx_set_gl(IMPLEMENTATION_COLOR_READ_TYPE);
    ctx_set_gl(IMPLEMENTATION_COLOR_READ_FORMAT);
    ctx_set_gl(COMPILE_STATUS);
    ctx_set_gl(INFO_LOG_LENGTH);
    ctx_set_gl(SHADER_SOURCE_LENGTH);
    ctx_set_gl(SHADER_COMPILER);
    ctx_set_gl(SHADER_BINARY_FORMATS);
    ctx_set_gl(NUM_SHADER_BINARY_FORMATS);
    ctx_set_gl(LOW_FLOAT);
    ctx_set_gl(MEDIUM_FLOAT);
    ctx_set_gl(HIGH_FLOAT);
    ctx_set_gl(LOW_INT);
    ctx_set_gl(MEDIUM_INT);
    ctx_set_gl(HIGH_INT);
    ctx_set_gl(FRAMEBUFFER);
    ctx_set_gl(RENDERBUFFER);
    ctx_set_gl(RGBA4);
    ctx_set_gl(RGB5_A1);
    ctx_set_gl(RGB565);
    ctx_set_gl(DEPTH_COMPONENT16);
    ctx_set_gl(STENCIL_INDEX8);
    ctx_set_gl(RENDERBUFFER_WIDTH);
    ctx_set_gl(RENDERBUFFER_HEIGHT);
    ctx_set_gl(RENDERBUFFER_INTERNAL_FORMAT);
    ctx_set_gl(RENDERBUFFER_RED_SIZE);
    ctx_set_gl(RENDERBUFFER_GREEN_SIZE);
    ctx_set_gl(RENDERBUFFER_BLUE_SIZE);
    ctx_set_gl(RENDERBUFFER_ALPHA_SIZE);
    ctx_set_gl(RENDERBUFFER_DEPTH_SIZE);
    ctx_set_gl(RENDERBUFFER_STENCIL_SIZE);
    ctx_set_gl(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE);
    ctx_set_gl(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME);
    ctx_set_gl(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
    ctx_set_gl(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE);
    ctx_set_gl(COLOR_ATTACHMENT0);
    ctx_set_gl(DEPTH_ATTACHMENT);
    ctx_set_gl(STENCIL_ATTACHMENT);
    ctx_set_gl(NONE);
    ctx_set_gl(FRAMEBUFFER_COMPLETE);
    ctx_set_gl(FRAMEBUFFER_INCOMPLETE_ATTACHMENT);
    ctx_set_gl(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT);
    ctx_set_gl(FRAMEBUFFER_INCOMPLETE_DIMENSIONS);
    ctx_set_gl(FRAMEBUFFER_UNSUPPORTED);
    ctx_set_gl(FRAMEBUFFER_BINDING);
    ctx_set_gl(RENDERBUFFER_BINDING);
    ctx_set_gl(MAX_RENDERBUFFER_SIZE);
    ctx_set_gl(INVALID_FRAMEBUFFER_OPERATION);
    
    ctx_set_gl(DEPTH_STENCIL);
    ctx_set_gl(DEPTH_STENCIL_ATTACHMENT);
    ctx_set_gl(UNPACK_FLIP_Y_WEBGL);
    ctx_set_gl(UNPACK_PREMULTIPLY_ALPHA_WEBGL);
    ctx_set_gl(CONTEXT_LOST_WEBGL);
    ctx_set_gl(UNPACK_COLORSPACE_CONVERSION_WEBGL);
    ctx_set_gl(BROWSER_DEFAULT_WEBGL);
    ctx_set_gl(UNMASKED_VENDOR_WEBGL);
    ctx_set_gl(UNMASKED_RENDERER_WEBGL);
    
    js_end_binding(WebGLRenderingContext);
}
