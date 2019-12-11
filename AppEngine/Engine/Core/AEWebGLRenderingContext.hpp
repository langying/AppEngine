//
//  AEWebGLRenderingContext.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/1/21.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEWebGLRenderingContext_hpp
#define AEWebGLRenderingContext_hpp

#include "AEPlatform.hpp"

class AEWebGLRenderingContext {
public:
    js_prop(AEWebGLRenderingContext, int,  fbo);
    js_prop(AEWebGLRenderingContext, bool, alpha);
    js_prop(AEWebGLRenderingContext, bool, antialias);
    js_prop(AEWebGLRenderingContext, bool, depth);
    js_prop(AEWebGLRenderingContext, bool, stencil);
    js_prop(AEWebGLRenderingContext, uint, packAlignment);
    js_prop(AEWebGLRenderingContext, bool, premultipliedAlpha);
    js_prop(AEWebGLRenderingContext, bool, preserveDrawingBuffer);
    js_prop(AEWebGLRenderingContext, bool, unpackFlipY);
    js_prop(AEWebGLRenderingContext, uint, unpackAlignment);
    js_prop(AEWebGLRenderingContext, bool, unpackPremultiplyAlpha);
    js_prop(AEWebGLRenderingContext, bool, unpackColorspaceConversion);
    
    js_con_destructor(AEWebGLRenderingContext, [](duk_context* ctx, AEWebGLRenderingContext* thiz, GLint argc) {});
    
public:
    js_func(size); // canvas大小变化时，朗英会调用此接口

public: // WebGL New API
    js_func(clearDepth);
    js_func(createBuffer);
    js_func(createFramebuffer);
    js_func(createRenderbuffer);
    js_func(createTexture);
    js_func(deleteBuffer);
    js_func(deleteFramebuffer);
    js_func(deleteRenderbuffer);
    js_func(deleteTexture);
    js_func(depthRange);
    js_func(getBufferParameter);
    js_func(getContextAttributes);
    js_func(getExtension);
    js_func(getFramebufferAttachmentParameter);
    js_func(getParameter);
    js_func(getProgramParameter);
    js_func(getRenderbufferParameter);
    js_func(getShaderParameter);
    js_func(getSupportedExtensions);
    js_func(getTexParameter);
    js_func(getUniform);
    js_func(getVertexAttrib);
    js_func(getVertexAttribOffset);
    js_func(isContextLost);
    
public: // OpenGL ES 2.0 API
    js_func(activeTexture);
    js_func(attachShader);
    js_func(bindAttribLocation);
    js_func(bindBuffer);
    js_func(bindFramebuffer);
    js_func(bindRenderbuffer);
    js_func(bindTexture);
    js_func(blendColor);
    js_func(blendEquation);
    js_func(blendEquationSeparate);
    js_func(blendFunc);
    js_func(blendFuncSeparate);
    js_func(bufferData);
    js_func(bufferSubData);
    js_func(checkFramebufferStatus);
    js_func(clear);
    js_func(clearColor);
    js_func(clearDepthf);
    js_func(clearStencil);
    js_func(colorMask);
    js_func(compileShader);
    js_func(compressedTexImage2D);
    js_func(compressedTexSubImage2D);
    js_func(copyTexImage2D);
    js_func(copyTexSubImage2D);
    js_func(createProgram);
    js_func(createShader);
    js_func(cullFace);
    js_func(deleteBuffers);
    js_func(deleteFramebuffers);
    js_func(deleteProgram);
    js_func(deleteRenderbuffers);
    js_func(deleteShader);
    js_func(deleteTextures);
    js_func(depthFunc);
    js_func(depthMask);
    js_func(depthRangef);
    js_func(detachShader);
    js_func(disable);
    js_func(disableVertexAttribArray);
    js_func(drawArrays);
    js_func(drawElements);
    js_func(enable);
    js_func(enableVertexAttribArray);
    js_func(finish);
    js_func(flush);
    js_func(framebufferRenderbuffer);
    js_func(framebufferTexture2D);
    js_func(frontFace);
    js_func(genBuffers);
    js_func(generateMipmap);
    js_func(genFramebuffers);
    js_func(genRenderbuffers);
    js_func(genTextures);
    js_func(getActiveAttrib);
    js_func(getActiveUniform);
    js_func(getAttachedShaders);
    js_func(getAttribLocation);
    js_func(getBooleanv);
    js_func(getBufferParameteriv);
    js_func(getError);
    js_func(getFloatv);
    js_func(getFramebufferAttachmentParameteriv);
    js_func(getIntegerv);
    js_func(getProgramiv);
    js_func(getProgramInfoLog);
    js_func(getRenderbufferParameteriv);
    js_func(getShaderiv);
    js_func(getShaderInfoLog);
    js_func(getShaderPrecisionFormat);
    js_func(getShaderSource);
    js_func(getString);
    js_func(getTexParameterfv);
    js_func(getTexParameteriv);
    js_func(getUniformfv);
    js_func(getUniformiv);
    js_func(getUniformLocation);
    js_func(getVertexAttribfv);
    js_func(getVertexAttribiv);
    js_func(getVertexAttribPointerv);
    js_func(hint);
    js_func(isBuffer);
    js_func(isEnabled);
    js_func(isFramebuffer);
    js_func(isProgram);
    js_func(isRenderbuffer);
    js_func(isShader);
    js_func(isTexture);
    js_func(lineWidth);
    js_func(linkProgram);
    js_func(pixelStorei);
    js_func(polygonOffset);
    js_func(readPixels);
    js_func(releaseShaderCompiler);
    js_func(renderbufferStorage);
    js_func(sampleCoverage);
    js_func(scissor);
    js_func(shaderBinary);
    js_func(shaderSource);
    js_func(stencilFunc);
    js_func(stencilFuncSeparate);
    js_func(stencilMask);
    js_func(stencilMaskSeparate);
    js_func(stencilOp);
    js_func(stencilOpSeparate);
    js_func(texImage2D);
    js_func(texParameterf);
    js_func(texParameterfv);
    js_func(texParameteri);
    js_func(texParameteriv);
    js_func(texSubImage2D);
    js_func(uniform1f);
    js_func(uniform1fv);
    js_func(uniform1i);
    js_func(uniform1iv);
    js_func(uniform2f);
    js_func(uniform2fv);
    js_func(uniform2i);
    js_func(uniform2iv);
    js_func(uniform3f);
    js_func(uniform3fv);
    js_func(uniform3i);
    js_func(uniform3iv);
    js_func(uniform4f);
    js_func(uniform4fv);
    js_func(uniform4i);
    js_func(uniform4iv);
    js_func(uniformMatrix2fv);
    js_func(uniformMatrix3fv);
    js_func(uniformMatrix4fv);
    js_func(useProgram);
    js_func(validateProgram);
    js_func(vertexAttrib1f);
    js_func(vertexAttrib1fv);
    js_func(vertexAttrib2f);
    js_func(vertexAttrib2fv);
    js_func(vertexAttrib3f);
    js_func(vertexAttrib3fv);
    js_func(vertexAttrib4f);
    js_func(vertexAttrib4fv);
    js_func(vertexAttribPointer);
    js_func(viewport);
};

#endif /* AEWebGLRenderingContext_hpp */
