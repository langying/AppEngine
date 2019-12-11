//
//  AEVector4.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/7.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEVector4_hpp
#define AEVector4_hpp

#include "AEPlatform.hpp"

void  vec4_set(float* dst, float x, float y, float z, float w);
void  vec4_setScalar(float* dst, float scalar);
void  vec4_copy(float* dst, const float* src);
void  vec4_add(float* dst, const float* src);
void  vec4_addScalar(float* dst, float scalar);
void  vec4_addVectors(float* dst, const float* a, const float* b);
void  vec4_addScaledVector(float* dst, const float* src, float scalar);
void  vec4_sub(float* dst, const float* src);
void  vec4_subScalar(float* dst, float scalar);
void  vec4_subVectors(float* dst, const float* a, const float* b);
void  vec4_multiplyScalar(float* dst, float scalar);
void  vec4_applyMatrix4(float* dst, const float* m4);
void  vec4_divideScalar(float* dst, float scalar);
void  vec4_setAxisAngleFromQuaternion(float* dst, const float* quaternion);
void  vec4_setAxisAngleFromRotationMatrix(float* dst, const float* m4);
void  vec4_min(float* dst, const float* src);
void  vec4_max(float* dst, const float* src);
void  vec4_clamp(float* dst, const float* min, const float* max);
void  vec4_clampScalar(float* dst, float min, float max);
void  vec4_floor(float* dst);
void  vec4_ceil(float* dst);
void  vec4_round(float* dst);
void  vec4_roundToZero(float* dst);
void  vec4_negate(float* dst);
float vec4_dot(const float* a, const float* b);
float vec4_length(const float* dst);
float vec4_lengthSq(const float* dst);
float vec4_lengthManhattan(const float* dst);
void  vec4_normalize(float* dst);
void  vec4_setLength(float* dst, float length);
void  vec4_lerp(float* dst, const float* v4, float alpha);
void  vec4_lerpVectors(float* dst, const float* v1, const float* v2, float alpha);
bool  vec4_equals(const float* a, const float* b);
void  vec4_fromArray(float* dst, const float* array, int offset);
void  vec4_toArray(const float* dst, float* array, int offset);

#endif /* AEVector4_hpp */
