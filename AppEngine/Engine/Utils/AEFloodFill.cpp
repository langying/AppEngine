//
//  AEFloodFill.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/22.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEFloodFill.hpp"

bool needToFill(AEColor pen, AEColor dst, AEColor* img, int idx, int len, uint8_t tolerance) {
    if (idx < 0 || idx >= len) {
        return false;   //out of bounds
    }
    if (img[idx].rgba.a == 0 && pen.rgba.a > 0) {
        return true;    //surface is invisible and fill is visible
    }
    if (abs(dst.rgba.a - pen.rgba.a) <= tolerance &&
        abs(dst.rgba.r - pen.rgba.r) <= tolerance &&
        abs(dst.rgba.g - pen.rgba.g) <= tolerance &&
        abs(dst.rgba.b - pen.rgba.b) <= tolerance) {
        return false;   //target is same as fill
    }
    if (dst.hex == img[idx].hex) {
        return true;    //target matches surface
    }
    if (abs(dst.rgba.a - img[idx].rgba.a) <= (255-tolerance) &&
        abs(dst.rgba.r - img[idx].rgba.r) <= tolerance       &&
        abs(dst.rgba.g - img[idx].rgba.g) <= tolerance       &&
        abs(dst.rgba.b - img[idx].rgba.b) <= tolerance) {
        return true;    //target to surface within tolerance
    }
    return false;       //no match
}

bool fillIfNeed(AEColor pen, AEColor dst, AEColor* img, int idx, int len, uint8_t tolerance) {
    if (needToFill(pen, dst, img, idx, len, tolerance)) {
        //fill the color
        img[idx] = pen;
        return true;
    }
    return false;
}

void floodfill(AEColor pen, AEColor* img, int x, int y, int w, int h, uint8_t tolerance) {
    std::vector<int> queue;
    int len = w * h;
    int idx = x + y * w;
    int beg, end, dx1, dx2;
    
    AEColor dst = img[idx];
    if (!needToFill(pen, dst, img, idx, len, tolerance)) {
        return;
    }
    queue.push_back(idx);
    while(queue.size()) {
        idx = queue.back();
        queue.pop_back();
        if (fillIfNeed(pen, dst, img, idx, len, tolerance)) {
        	dx1 = idx;
            dx2 = idx;
            beg = (idx / w) * w;  // left bound
            end = beg + w;   // right bound
            while (beg < dx1 && beg < (--dx1) && fillIfNeed(pen, dst, img, dx1, len, tolerance)); //go left until edge hit
            while (end > dx2 && end > (--dx2) && fillIfNeed(pen, dst, img, dx2, len, tolerance)); //go right until edge hit
            for (int i = dx1; i < dx2; i++) {
                if (i-w>=0  && needToFill(pen, dst, img, i-w, len, tolerance)) queue.push_back(i-w); //queue y-1
                if (i+w<len && needToFill(pen, dst, img, i+w, len, tolerance)) queue.push_back(i+w); //queue y+1
            }
        }
    }
}
