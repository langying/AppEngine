//
//  AEBase.cpp
//  AppEngine
//
//  Created by 韩琼 on 16/4/25.
//  Copyright © 2016年 Amaze. All rights reserved.
//

#include <math.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "AEBase.hpp"

double get_now() {
    static struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((double)tv.tv_sec) * 1000.0 + ((double)tv.tv_usec) / 1000.0;
}

double deg2rad(double d) {
    return d * (M_PI / 180);
}
double rad2deg(double r) {
    return r / (M_PI / 180);
}
float deg2radf(float d) {
    return d * (M_PI / 180);
}
float rad2degf(float r) {
    return r / (M_PI / 180);
}
float euclideanModulo(float n, float m) {
    return fmodf((fmodf(n, m) + m), m);
}

float lerpf(float a, float b, float t) {
    return ( 1 - t ) * a + t * b;
}
float fmaxf(float a, float b, float c) {
    return fmaxf(a, fmaxf(b, c));
}
float fminf(float a, float b, float c) {
    return fminf(a, fminf(b, c));
}

float clampf(float val, float min, float max) {
    return fmaxf(min, fminf(val, max));
}
float slidef(float val, float min, float max) {
    return min + (max - min) * clampf(val, 0, 1);
}
bool insidef(float val, float min, float max) {
    return val >= min && val <= max;
}
float mapLinear(float x, float a1, float a2, float b1, float b2) {
    return b1 + ( x - a1 ) * ( b2 - b1 ) / ( a2 - a1 );
}

AEData AEDataWithFile(const char* pathfile) {
    AEData data = { 0 };
    if (pathfile) {
        FILE* file  = fopen(pathfile, "rb");
        fseek(file, 0, SEEK_END);
        data.length = static_cast<uint>(ftell(file));
        fseek(file, 0, SEEK_SET);
        data.buffer = calloc(data.length + 1, sizeof(char));
        fread(data.buffer, sizeof(char), data.length, file);
        fclose(file);
    }
    return data;
}

int lengthWithUTFString(const char* s) {
    int idx = 0, num = 0;
    while (s[idx]) {
        if ((s[idx] & 0xC0) != 0x80) {
            num++;
        }
        idx++;
    }
    return num;
}
int lengthWithStdString(std::string& str) {
    return lengthWithUTFString(str.c_str());
}
std::string& trim(std::string &str) {
    if (str.empty()) {
        return str;
    }
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ') + 1);
    return str;
}
void split(std::string& str, std::string& delim, std::vector<std::string>& array) {
    size_t iseek  = 0;
    size_t index = str.find_first_of(delim, iseek);
    while (index != std::string::npos) {
        array.push_back(str.substr(iseek, index - iseek));
        iseek = index + 1;
        index = str.find_first_of(delim, iseek);
    }
    if (index - iseek > 0) {
        array.push_back(str.substr(iseek, index - iseek));
    }
}
