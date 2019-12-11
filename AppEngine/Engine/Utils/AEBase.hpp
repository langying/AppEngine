//
//  AEBase.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/4/25.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#ifndef AEBase_hpp
#define AEBase_hpp

#include <vector>
#include <Endian.h>
#include <sys/types.h>

namespace AEEvent {
enum Type {
    KeyUp = 1,
    KeyDown,
    MouseUp,
    MouseOut,
    MouseDown,
    MouseMove,
    TouchEnd,
    TouchMove,
    TouchStart,
    TouchCancel
};
};

int64_t a;

typedef struct {
    uint  length;
    void* buffer;
} AEData;

typedef union _AEColor {
    uint32_t  hex;
    uint8_t   data[4];
    struct {
#if __BYTE_ORDER == __BIG_ENDIAN
        uint8_t a, b, g, r;
#else
        uint8_t r, g, b, a;
#endif
    } rgba;
} AEColor;

typedef bool        boolean;
typedef int64_t     Integer;

double get_now();

double deg2rad(double d);
double rad2deg(double r);

float deg2radf(float d);
float rad2degf(float r);

float euclideanModulo(float n, float m);

float lerpf(float a, float b, float t);
float fmaxf(float a, float b, float c);
float fminf(float a, float b, float c);

float clampf(float val, float min, float max);
float slidef(float val, float min, float max);
bool  insidef(float val, float min, float max);
float mapLinear(float x, float a1, float a2, float b1, float b2);

AEData AEDataWithFile(const char* pathfile);

int lengthWithUTFString(const char* str);
int lengthWithStdString(std::string& str);

std::string& trim(std::string& str);
void split(std::string& str, std::string& delim, std::vector<std::string>& array);

#endif /* AEBase_hpp */
