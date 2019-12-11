//
//  AEMatrix3.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/7.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEVector3.hpp"
#include "AEMatrix3.hpp"

void mat3_set(float* te, float n11, float n12, float n13, float n21, float n22, float n23, float n31, float n32, float n33) {
    te[ 0 ] = n11; te[ 1 ] = n21; te[ 2 ] = n31;
    te[ 3 ] = n12; te[ 4 ] = n22; te[ 5 ] = n32;
    te[ 6 ] = n13; te[ 7 ] = n23; te[ 8 ] = n33;
}
void mat3_copy(float* dst, const float* src) {
    memcpy(dst, src, sizeof(float) * 9);
}
void mat3_identity(float* m3) {
    mat3_set(m3,
             1, 0, 0,
             0, 1, 0,
             0, 0, 1);
}
void mat3_setFromMatrix4(float* m3, const float* me) {
    mat3_set(m3,
             me[ 0 ], me[ 4 ], me[  8 ],
             me[ 1 ], me[ 5 ], me[  9 ],
             me[ 2 ], me[ 6 ], me[ 10 ]);
}
void mat3_applyToVector3Array(const float* m3, float* array, int offset, int length) {
    static float v1[3];
    for ( int i = 0, j = offset; i < length; i += 3, j += 3 ) {
        vec3_fromArray(v1, array, j);
        vec3_applyMatrix3(v1, m3);
        vec3_toArray(v1, array, j);
    }
}
void mat3_multiplyScalar(float* te, float s) {
    te[ 0 ] *= s; te[ 3 ] *= s; te[ 6 ] *= s;
    te[ 1 ] *= s; te[ 4 ] *= s; te[ 7 ] *= s;
    te[ 2 ] *= s; te[ 5 ] *= s; te[ 8 ] *= s;
}
float mat3_determinant(const float* te) {
    float a = te[ 0 ], b = te[ 1 ], c = te[ 2 ];
    float d = te[ 3 ], e = te[ 4 ], f = te[ 5 ];
    float g = te[ 6 ], h = te[ 7 ], i = te[ 8 ];
    return a * e * i - a * f * h - b * d * i + b * f * g + c * d * h - c * e * g;
}
void mat3_getInverse(float* te, const float* me) {
    float n11 = me[ 0 ], n21 = me[ 1 ], n31 = me[ 2 ];
    float n12 = me[ 3 ], n22 = me[ 4 ], n32 = me[ 5 ];
    float n13 = me[ 6 ], n23 = me[ 7 ], n33 = me[ 8 ];
    
    float t11 = n33 * n22 - n32 * n23;
    float t12 = n32 * n13 - n33 * n12;
    float t13 = n23 * n12 - n22 * n13;
    
    float det = n11 * t11 + n21 * t12 + n31 * t13;
    
    if (det == 0) {
        LOG("THREE.Matrix3.getInverse(): can't invert matrix, determinant is 0");
        mat3_identity(te);
        return;
    }
    
    float detInv = 1 / det;
    
    te[ 0 ] = t11 * detInv;
    te[ 1 ] = ( n31 * n23 - n33 * n21 ) * detInv;
    te[ 2 ] = ( n32 * n21 - n31 * n22 ) * detInv;
    
    te[ 3 ] = t12 * detInv;
    te[ 4 ] = ( n33 * n11 - n31 * n13 ) * detInv;
    te[ 5 ] = ( n31 * n12 - n32 * n11 ) * detInv;
    
    te[ 6 ] = t13 * detInv;
    te[ 7 ] = ( n21 * n13 - n23 * n11 ) * detInv;
    te[ 8 ] = ( n22 * n11 - n21 * n12 ) * detInv;
}
void mat3_transpose(float* m) {
    float tmp;
    tmp = m[ 1 ]; m[ 1 ] = m[ 3 ]; m[ 3 ] = tmp;
    tmp = m[ 2 ]; m[ 2 ] = m[ 6 ]; m[ 6 ] = tmp;
    tmp = m[ 5 ]; m[ 5 ] = m[ 7 ]; m[ 7 ] = tmp;
}
void mat3_getNormalMatrix(float* m3, const float* m4) {
    mat3_setFromMatrix4(m3, m4);
    mat3_getInverse(m3, m3);
    mat3_transpose(m3);
}
void mat3_transposeIntoArray(const float* src, float* dst) {
    memcpy(dst, src, sizeof(float) * 9);
}
void mat3_fromArray(float* m3, const float* array, int offset) {
    array += offset;
    memcpy(m3, array, sizeof(float) * 9);
}
void mat3_toArray(const float* m3, float* array, int offset) {
    array += 9;
    memcpy(array, m3, sizeof(float) * 9);
}
