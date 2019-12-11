//
//  AEMatrix4.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEMatrix4_hpp
#define AEMatrix4_hpp

#include "AEVector3.hpp"
#include "AEQuaternion.hpp"

void  mat4_set(float *m,
               float n11, float n12, float n13, float n14,
               float n21, float n22, float n23, float n24,
               float n31, float n32, float n33, float n34,
               float n41, float n42, float n43, float n44);
void  mat4_identity(float* m);
void  mat4_copy(float* dst, const float* src);
void  mat4_extractRotation(float* dst, const float* src);
void  mat4_makeRotationFromEuler(float* te, const float* euler, std::string order);
void  mat4_makeRotationFromQuaternion(float* dst, const float* quaternion);
void  mat4_lookAt(float* dst, float* eye, float* target, float* up);
void  mat4_multiplyScalar(float* dst, float scale);
void  mat4_multiplyMatrices(float* dst, const float* a, const float* b);
void  mat4_applyToVector3Array(const float* m, float* array, int offset, int length);
float mat4_determinant(const float* m);
void  mat4_transpose(float* dst);
void  mat4_setPosition(float* dst, const float* v3);
void  mat4_getInverse(float* dst, const float* src);
void  mat4_scale(float* dst, const float* v3);
float mat4_getMaxScaleOnAxis(const float* m);
void  mat4_makeTranslation(float* m, float x, float y, float z);
void  mat4_makeRotationX(float* dst, float theta);
void  mat4_makeRotationY(float* dst, float theta);
void  mat4_makeRotationZ(float* dst, float theta);
void  mat4_makeRotationAxis(float* dst, const float* v3, float angle);
void  mat4_makeScale(float* m, float x, float y, float z);
void  mat4_makeShear(float* dst, float x, float y, float z);
void  mat4_compose(float* dst, const float* position, const float* quaternion, const float* scale);
void  mat4_makePerspective(float* dst, float left, float right, float top, float bottom, float near, float far);
void  mat4_makeOrthographic(float* dst, float left, float right, float top, float bottom, float near, float far);
bool  mat4_equals(const float* a, const float* b);
void  mat4_fromArray(float* dst, const float* array, int offset);
void  mat4_toArray(const float* dst, float* array, int offset);

#endif /* AEMatrix4_hpp */
