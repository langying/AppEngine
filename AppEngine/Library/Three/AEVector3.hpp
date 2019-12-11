//
//  AEVector3.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEVector3_hpp
#define AEVector3_hpp

#include "AEPlatform.hpp"

void  vec3_set(float* v, float x, float y, float z);
void  vec3_setScalar(float* dst, float scalar);
void  vec3_copy(float* dst, const float* src);
void  vec3_add(float* dst, const float* src);
void  vec3_addScalar(float* dst, float scalar);
void  vec3_addVectors(float* dst, const float* a, const float* b);
void  vec3_addScaledVector(float* dst, const float* src, float scalar);
void  vec3_sub(float* dst, const float* src);
void  vec3_subScalar(float* dst, float scalar);
void  vec3_subVectors(float* dst, const float* a, const float* b);
void  vec3_multiply(float* dst, const float* src);
void  vec3_multiplyScalar(float* dst, float scalar);
void  vec3_multiplyVectors(float* dst, const float* a, const float* b);
void  vec3_applyEuler(float* dst, const float* euler, std::string order);
void  vec3_applyAxisAngle(float* dst, const float* axis, float angle);
void  vec3_applyMatrix3(float* dst, const float* m3);
void  vec3_applyMatrix4(float* dst, const float* m4);
void  vec3_applyProjection(float* dst, const float* m4);
void  vec3_applyQuaternion(float* dst, const float* quaternion);
void  vec3_project(float* dst, const float* projectionMatrix, const float* matrixWorld);
void  vec3_unproject(float* dst, const float* matrixWorld, const float* projectionMatrix);
void  vec3_transformDirection(float* dst, const float* m4);
void  vec3_divide(float* dst, const float* src);
void  vec3_divideScalar(float* dst, float scalar);
void  vec3_min(float* dst, const float* src);
void  vec3_max(float* dst, const float* src);
void  vec3_clamp(float* dst, const float* min, const float* max);
void  vec3_clampScalar(float* dst, float min, float max);
void  vec3_clampLength(float* dst, float min, float max);
void  vec3_floor(float* dst);
void  vec3_ceil(float* dst);
void  vec3_round(float* dst);
void  vec3_roundToZero(float* dst);
void  vec3_negate(float* dst);
float vec3_dot(const float* a, const float* b);
float vec3_length(const float* v);
float vec3_lengthSq(const float* v);
float vec3_lengthManhattan(const float* v);
void  vec3_normalize(float* dst);
void  vec3_setLength(float* dst, float length);
void  vec3_lerp(float* dst, const float* src, float alpha);
void  vec3_lerpVectors(float* dst, const float* v1, const float* v2, float alpha);
void  vec3_cross(float* dst, const float* src);
void  vec3_crossVectors(float* dst, const float* a, const float* b);
void  vec3_projectOnVector(float* dst, const float* v3);
void  vec3_projectOnPlane(float* dst, const float* v3);
void  vec3_reflect(float* dst, const float* v3);
float vec3_angleTo(const float* v1, const float* v2);
float vec3_distanceTo(const float* v1, const float* v2);
float vec3_distanceToSquared(const float* v1, const float* v2);
float vec3_distanceToManhattan(const float* v1, const float* v2);
void  vec3_setFromMatrixPosition(float* dst, const float* m4);
void  vec3_setFromMatrixScale(float* dst, const float* m4);
void  vec3_setFromMatrixColumn(float* dst, const float* m4, int index);
bool  vec3_equals(const float* a, const float* b);
void  vec3_fromArray(float* dst, const float* array, int offset);
void  vec3_toArray(const float* v, float* array, int offset);

#endif /* AEVector3_hpp */
