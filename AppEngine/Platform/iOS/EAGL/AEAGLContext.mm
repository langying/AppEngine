//
//  AEAGLContext.m
//  AppEngine
//
//  Created by 韩琼 on 2017/2/3.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#import <GLKit/GLKit.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "AEAGLContext.h"
#import "AEPlatform.hpp"

#define GLCheckError();                                         \
for (GLenum error = glGetError(); error; error = glGetError()) {\
    LOG("glError:0x%x\n", error);                            \
}

const char VSH[] = ""
"attribute vec4 aPosition;                  "
"attribute vec2 aTexCoord;                  "
"varying   vec2 vTexCoord;                  "
"uniform   mat4 uTexMatrix;                 "
"void main() {                              "
"    vTexCoord   = aTexCoord;               "
"    gl_Position = aPosition * uTexMatrix;  "
"}";

const char FSH[] = ""
"precision mediump float;                           "
"varying vec2       vTexCoord;                      "
"uniform sampler2D  uTexture;                       "
"void main() {                                      "
"    gl_FragColor = texture2D(uTexture, vTexCoord); "
"}";

GLfloat mPosIndexs[] = {
    // X, Y
    -1.0f, -1.0f,	// LB
    +1.0f, -1.0f,	// RB
    -1.0f, +1.0f,	// LT
    +1.0f, +1.0f,	// RT
};

GLfloat mTexIndexs[] = {
    // X, Y
    0.0f, 0.0f,	// LB
    1.0f, 0.0f,	// RB
    0.0f, 1.0f,	// LT
    1.0f, 1.0f,	// RT
};

@interface AEAGLContext()

@property(nonatomic, assign) GLuint FBO;
@property(nonatomic, assign) GLuint TEX;
@property(nonatomic, assign) GLuint program;
@property(nonatomic, assign) GLint  aPosition;
@property(nonatomic, assign) GLint  aTexCoord;
@property(nonatomic, assign) GLint  uTexMatrix;

@property(nonatomic, retain) EAGLContext*              context;
@property(nonatomic, assign) CVOpenGLESTextureRef      texture;
@property(nonatomic, assign) CVOpenGLESTextureCacheRef texCache;

@end

@implementation AEAGLContext

- (void)dealloc {
    CVOpenGLESTextureCacheFlush(_texCache, 0);
    if (_texture)  { CFRelease(_texture);  }
    if (_texCache) { CFRelease(_texCache); }
    if (_TEX)      { glDeleteTextures(1, &_TEX);     }
    if (_FBO)      { glDeleteFramebuffers(1, &_FBO); }
    glDeleteProgram(_program);
    
    self.context  = nil;
    self.texture  = nil;
    self.texCache = nil;
    [super dealloc];
}

- (id)init {
    if ((self = [super init]) == nil) {
        return nil;
    }
    
    EAGLContext* cx = [EAGLContext currentContext];
    self.context = ({
        EAGLContext* context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2 sharegroup:cx.sharegroup] autorelease];
        [EAGLContext setCurrentContext:context];
        context;
    });

    _program    = [self programWithVSH:VSH FSH:FSH];
    _aPosition  = glGetAttribLocation( _program, "aPosition");
    _aTexCoord  = glGetAttribLocation( _program, "aTexCoord");
    _uTexMatrix = glGetUniformLocation(_program, "uTexMatrix");

    CVReturn error = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, _context, NULL, &_texCache);
    if (error != noErr) {
        LOG("CVOpenGLESTextureCacheCreate error:%d", error);
    }
    [EAGLContext setCurrentContext:cx];
    return self;
}

- (void)makeCurrent {
    if ([EAGLContext currentContext] != _context) {
        [EAGLContext setCurrentContext:_context];
    }
}

- (void)drawTarget:(GLuint)texId buffer:(CVPixelBufferRef)buffer matrix:(GLfloat*)matrix {
    [self makeCurrent];
    size_t width  = CVPixelBufferGetWidth(buffer);
    size_t height = CVPixelBufferGetHeight(buffer);
    
    if (_TEX != texId) {
        if (_FBO) {
            glDeleteFramebuffers(1, &_FBO);
            _FBO = 0;
        }
        _TEX = texId;
        glBindTexture(GL_TEXTURE_2D, _TEX);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        
        glGenFramebuffers(1, &_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _TEX, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOG("texImage2D::glCheckFramebufferStatus failure.");
        }
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
    glViewport(0, 0, width, height);
    glActiveTexture(GL_TEXTURE0);
    CVReturn error = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, _texCache, buffer, NULL,
                         GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA, GL_UNSIGNED_BYTE, 0, &_texture);
    if (error) {
        LOG("Error at CVOpenGLESTextureCacheCreateTextureFromImage %d", error);
    }
    
    glBindTexture(CVOpenGLESTextureGetTarget(_texture), CVOpenGLESTextureGetName(_texture));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glUseProgram(_program);
    GLCheckError();
    glUniformMatrix4fv(_uTexMatrix, 1, GL_FALSE, matrix);
    glVertexAttribPointer(_aPosition, 2, GL_FLOAT, 0, 0, mPosIndexs);
    glEnableVertexAttribArray(_aPosition);
    glVertexAttribPointer(_aTexCoord, 2, GL_FLOAT, 0, 0, mTexIndexs);
    glEnableVertexAttribArray(_aTexCoord);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glFinish();
    
    if (_texture) {
        CFRelease(_texture);
        _texture = nil;
    }
    CVOpenGLESTextureCacheFlush(_texCache, 0);
}

- (GLuint)programWithVSH:(const char*)vsh FSH:(const char*)fsh {
    GLuint vertexShader = [self shaderWithType:GL_VERTEX_SHADER source:vsh];
    if (vertexShader == 0) {
        return 0;
    }
    GLuint fragmentShader = [self shaderWithType:GL_FRAGMENT_SHADER source:fsh];
    if (fragmentShader == 0) {
        return 0;
    }
    
    GLuint program = glCreateProgram();
    if (program) {
        GLint status = GL_FALSE;
        glAttachShader(program, vertexShader);
        GLCheckError();
        glAttachShader(program, fragmentShader);
        GLCheckError();
        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_TRUE) {
            if (vertexShader) {
                glDetachShader(program, vertexShader);
                glDeleteShader(vertexShader);
            }
            if (fragmentShader) {
                glDetachShader(program, fragmentShader);
                glDeleteShader(fragmentShader);
            }
        }
        else {
            GLCheckError();
            GLint size = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &size);
            GLchar* data = (GLchar*)malloc(size);
            LOG("Error when link program:%s", data);
            free(data);
            if (program) {
                glDeleteProgram(program);
                program = 0;
            }
            if (vertexShader) {
                glDeleteShader(vertexShader);
            }
            if (fragmentShader) {
                glDeleteShader(fragmentShader);
            }
        }
    }
    return program;
}

- (GLuint)shaderWithType:(GLenum)shaderType source:(const char*)source {
    GLuint shader = glCreateShader(shaderType);
    if (shader == 0) {
        return shader;
    }
    
    GLint success = GL_FALSE;
    glShaderSource(shader, 1, &source, nil);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLCheckError();
        GLint size = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &size);
        if (size) {
            GLchar* data = (GLchar*)malloc(size);
            glGetShaderInfoLog(shader, size, NULL, data);
            LOG("Error Compiling Shader:\n %s\n", data);
            free(data);
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

@end
