//
//  AECanvasRenderingContext2DBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/9/12.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AECanvasRenderingContext2DBean_hpp
#define AECanvasRenderingContext2DBean_hpp

#include "AEPlatform.hpp"

class AEHTMLImageElement;
class AECanvasRenderingContext2DBean {
public:
	jobject  impl;
	GLuint   _width;		GLvoid setWidth(GLuint val);		GLuint   getWidth()		{ return _width;     }
	GLuint   _height;		GLvoid setHeight(GLuint val);		GLuint   getHeight()	{ return _height;    }
	GLuint   _fillColor;	GLvoid setFillColor(GLuint val);	GLuint   getFillColor()	{ return _fillColor; }
	GLuint   _fontColor;	GLvoid setFontColor(GLuint val);	GLuint   getFontColor()	{ return _fontColor; }
	GLfloat  _fontSize;		GLvoid setFontSize(GLfloat val);	GLfloat  getFontSize()	{ return _fontSize;  }
	GLfloat  _lineWidth;	GLvoid setLineWidth(GLfloat val);	GLfloat  getLineWidth()	{ return _lineWidth; }
	GLstring _fontName;		GLvoid setFontName(GLstring val);	GLstring getFontName()	{ return _fontName;  }
	GLstring _fillStyle;	GLvoid setFillStyle(GLstring val);	GLstring getFillStyle() { return _fillStyle; }

public:
    AECanvasRenderingContext2DBean();
    ~AECanvasRenderingContext2DBean();
    void init(AEManager* mgr);
    void resize(int width, int height);
    void texImage2D(GLenum target, GLint level, GLint infmt, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    void texSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLenum format, GLenum type, bool unpackFlipY, bool unpackPremultiplyAlpha);
    
public:
    void  clearRect(int x, int y, int w, int h);
    void  drawImage(AEImage* img, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh);
    void  fillRect(int x, int y, int w, int h);
    void  fillText(const char* text, int x, int y);
    void  getImageData(int x, int y, int w, int h, void* pixels);
    float measureText(const char* text);
    void  scale(float x, float y);
    void  strokeText(const char* text, int x, int y);
};

#endif /* AECanvasRenderingContext2DBean_hpp */
