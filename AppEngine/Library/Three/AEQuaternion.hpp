//
//  AEQuaternion.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEQuaternion_hpp
#define AEQuaternion_hpp

#include "AEPlatform.hpp"

void  quat_set(float* q, float x, float y, float z, float w);
void  quat_copy(float* dst, const float* src);
void  quat_setFromEuler(float* q, const float* e, std::string order);
void  quat_setFromAxisAngle(float* q, const float* v3, float angle);
void  quat_setFromRotationMatrix(float* q, const float* m4);
void  quat_setFromUnitVectors(float* q, const float* v3From, const float* v3To);
void  quat_inverse(float* q);
void  quat_conjugate(float* q);
float quat_dot(const float* q, const float* v4);
float quat_length(const float* q);
float quat_lengthSq(const float* q);
void  quat_normalize(float* q);
void  multiplyQuaternions(float* q, const float* a, const float* b);
void  quat_slerp(float* q, const float* qb, float t);
bool  quat_equals(const float* a, const float* b);
void  quat_fromArray(float* q, const float* array, int offset);
void  quat_toArray(const float* q, float* array, int offset);

#endif /* AEQuaternion_hpp */
