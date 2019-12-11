//
//  AEMatrix3.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/7.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEMatrix3_hpp
#define AEMatrix3_hpp

#include "AEPlatform.hpp"

void  mat3_set(float* m3, float n11, float n12, float n13, float n21, float n22, float n23, float n31, float n32, float n33);
void  mat3_copy(float* dst, const float* src);
void  mat3_identity(float* m3);
void  mat3_setFromMatrix4(float* m3, const float* m4);
void  mat3_applyToVector3Array(const float* m3, float* array, int offset, int length);
void  mat3_multiplyScalar(float* m3, float scalar);
float mat3_determinant(const float* m3);
void  mat3_getInverse(float* dst, const float* src);
void  mat3_transpose(float* m3);
void  mat3_getNormalMatrix(float* m3, const float* m4);
void  mat3_transposeIntoArray(const float* src, float* dst);
void  mat3_fromArray(float* m3, const float* array, int offset);
void  mat3_toArray(const float* m3, float* array, int offset);

#endif /* AEMatrix3_hpp */
