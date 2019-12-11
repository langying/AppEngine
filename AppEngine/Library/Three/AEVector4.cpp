//
//  AEVector4.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/7.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include <cmath>
#include "AEVector4.hpp"

void vec4_set(float* dst, float x, float y, float z, float w) {
    dst[0] = x;
    dst[1] = y;
    dst[2] = z;
    dst[3] = w;
}
void vec4_setScalar(float* dst, float scalar) {
    dst[0] = scalar;
    dst[1] = scalar;
    dst[2] = scalar;
    dst[3] = scalar;
}
void vec4_copy(float* dst, const float* src) {
    memcpy(dst, src, sizeof(float) * 4);
}
void vec4_add(float* dst, const float* v) {
    dst[0] += v[0];
    dst[1] += v[1];
    dst[2] += v[2];
    dst[3] += v[3];
}
void vec4_addScalar(float* dst, float s) {
    dst[0] += s;
    dst[1] += s;
    dst[2] += s;
    dst[3] += s;
}
void vec4_addVectors(float* dst, const float* a, const float* b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
    dst[3] = a[3] + b[3];
}
void vec4_addScaledVector(float* dst, const float* v, float s) {
    dst[0] += v[0] * s;
    dst[1] += v[1] * s;
    dst[2] += v[2] * s;
    dst[3] += v[3] * s;
}
void vec4_sub(float* dst, const float* v) {
    dst[0] -= v[0];
    dst[1] -= v[1];
    dst[2] -= v[2];
    dst[3] -= v[3];
}
void vec4_subScalar(float* dst, float s) {
    dst[0] -= s;
    dst[1] -= s;
    dst[2] -= s;
    dst[3] -= s;
}
void vec4_subVectors(float* dst, const float* a, const float* b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3] - b[3];
}
void vec4_multiplyScalar(float* dst, float scalar) {
    if (std::isfinite(scalar)) {
        dst[0] *= scalar;
        dst[1] *= scalar;
        dst[2] *= scalar;
        dst[3] *= scalar;
    }
    else {
        dst[0] = 0;
        dst[1] = 0;
        dst[2] = 0;
        dst[3] = 0;
    }
}
void vec4_applyMatrix4(float* dst, const float* e) {
    float x = dst[0], y = dst[1], z = dst[2], w = dst[3];
    
    dst[0] = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z + e[ 12 ] * w;
    dst[1] = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z + e[ 13 ] * w;
    dst[2] = e[ 2 ] * x + e[ 6 ] * y + e[10 ] * z + e[ 14 ] * w;
    dst[3] = e[ 3 ] * x + e[ 7 ] * y + e[11 ] * z + e[ 15 ] * w;
}
void vec4_divideScalar(float* dst, float scalar) {
    vec4_multiplyScalar(dst, 1 / scalar);
}
void vec4_setAxisAngleFromQuaternion(float* dst, const float* q) {
    dst[3] = 2 * acosf( q[3] );
    
    float s = sqrtf( 1 - q[3] * q[3] );
    
    if ( s < 0.0001 ) {
        
        dst[0] = 1;
        dst[1] = 0;
        dst[2] = 0;
        
    } else {
        
        dst[0] = q[0] / s;
        dst[1] = q[1] / s;
        dst[2] = q[2] / s;
        
    }
}
void vec4_setAxisAngleFromRotationMatrix(float* dst, const float* te) {
    float angle, x, y, z;		// variables for result
    float epsilon = 0.01;		// margin to allow for rounding errors
    float epsilon2 = 0.1;		// margin to distinguish between 0 and 180 degrees
    
    float m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ];
    float m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ];
    float m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[10 ];
    
    if (( fabsf( m12 - m21 ) < epsilon ) &&
        ( fabsf( m13 - m31 ) < epsilon ) &&
        ( fabsf( m23 - m32 ) < epsilon ) ){
        
        // singularity found
        // first check for identity matrix which must have +1 for all terms
        // in leading diagonal and zero in other terms
        
        if (( fabsf( m12 + m21 ) < epsilon2 ) &&
            ( fabsf( m13 + m31 ) < epsilon2 ) &&
            ( fabsf( m23 + m32 ) < epsilon2 ) &&
            ( fabsf( m11 + m22 + m33 - 3 ) < epsilon2 )) {
            
            // this singularity is identity matrix so angle = 0
            
            vec4_set(dst, 1, 0, 0, 0);
            
            return; // zero angle, arbitrary axis
            
        }
        
        // otherwise this singularity is angle = 180
        
        angle = M_PI;
        
        float xx = ( m11 + 1 ) / 2;
        float yy = ( m22 + 1 ) / 2;
        float zz = ( m33 + 1 ) / 2;
        float xy = ( m12 + m21 ) / 4;
        float xz = ( m13 + m31 ) / 4;
        float yz = ( m23 + m32 ) / 4;
        
        if ( ( xx > yy ) && ( xx > zz ) ) {
            
            // m11 is the largest diagonal term
            
            if ( xx < epsilon ) {
                
                x = 0;
                y = 0.707106781;
                z = 0.707106781;
                
            } else {
                
                x = sqrtf( xx );
                y = xy / x;
                z = xz / x;
                
            }
            
        } else if ( yy > zz ) {
            
            // m22 is the largest diagonal term
            
            if ( yy < epsilon ) {
                
                x = 0.707106781;
                y = 0;
                z = 0.707106781;
                
            } else {
                
                y = sqrtf( yy );
                x = xy / y;
                z = yz / y;
                
            }
            
        } else {
            
            // m33 is the largest diagonal term so base result on this
            
            if ( zz < epsilon ) {
                
                x = 0.707106781;
                y = 0.707106781;
                z = 0;
                
            } else {
                
                z = sqrtf( zz );
                x = xz / z;
                y = yz / z;
                
            }
            
        }
        
        vec4_set(dst, x, y, z, angle );
        
        return; // return 180 deg rotation
        
    }
    
    // as we have reached here there are no singularities so we can handle normally
    
    float s = sqrtf(( m32 - m23 ) * ( m32 - m23 ) +
                    ( m13 - m31 ) * ( m13 - m31 ) +
                    ( m21 - m12 ) * ( m21 - m12 ) ); // used to normalize
    
    if ( fabsf( s ) < 0.001 ) s = 1;
    
    // prevent divide by zero, should not happen if matrix is orthogonal and should be
    // caught by singularity test above, but I've left it in just in case
    
    dst[0] = ( m32 - m23 ) / s;
    dst[1] = ( m13 - m31 ) / s;
    dst[2] = ( m21 - m12 ) / s;
    dst[3] = acosf( ( m11 + m22 + m33 - 1 ) / 2 );
}
void vec4_min(float* dst, const float* v) {
    dst[0] = fminf( dst[0], v[0] );
    dst[1] = fminf( dst[1], v[1] );
    dst[2] = fminf( dst[2], v[2] );
    dst[3] = fminf( dst[3], v[3] );
}
void vec4_max(float* dst, const float* v) {
    dst[0] = fmaxf( dst[0], v[0] );
    dst[1] = fmaxf( dst[1], v[1] );
    dst[2] = fmaxf( dst[2], v[2] );
    dst[3] = fmaxf( dst[3], v[3] );
}
void vec4_clamp(float* dst, const float* min, const float* max) {
    dst[0] = fmaxf( min[0], fminf( max[0], dst[0] ) );
    dst[1] = fmaxf( min[1], fminf( max[1], dst[1] ) );
    dst[2] = fmaxf( min[2], fminf( max[2], dst[2] ) );
    dst[3] = fmaxf( min[3], fminf( max[3], dst[3] ) );
}
void vec4_clampScalar(float* dst, float minVal, float maxVal) {
    static float min[3], max[3];
    vec4_set(min, minVal, minVal, minVal, minVal);
    vec4_set(max, maxVal, maxVal, maxVal, maxVal);
    vec4_clamp(dst, min, max);
}
void vec4_floor(float* dst) {
    dst[0] = floorf( dst[0] );
    dst[1] = floorf( dst[1] );
    dst[2] = floorf( dst[2] );
    dst[3] = floorf( dst[3] );
}
void vec4_ceil(float* dst) {
    dst[0] = ceilf( dst[0] );
    dst[1] = ceilf( dst[1] );
    dst[2] = ceilf( dst[2] );
    dst[3] = ceilf( dst[3] );
}
void vec4_round(float* dst) {
    dst[0] = roundf( dst[0] );
    dst[1] = roundf( dst[1] );
    dst[2] = roundf( dst[2] );
    dst[3] = roundf( dst[3] );
}
void vec4_roundToZero(float* dst) {
    dst[0] = ( dst[0] < 0 ) ? ceilf( dst[0] ) : floorf( dst[0] );
    dst[1] = ( dst[1] < 0 ) ? ceilf( dst[1] ) : floorf( dst[1] );
    dst[2] = ( dst[2] < 0 ) ? ceilf( dst[2] ) : floorf( dst[2] );
    dst[3] = ( dst[3] < 0 ) ? ceilf( dst[3] ) : floorf( dst[3] );
}
void vec4_negate(float* dst) {
    dst[0] = - dst[0];
    dst[1] = - dst[1];
    dst[2] = - dst[2];
    dst[3] = - dst[3];
}
float vec4_dot(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
float vec4_length(const float* v) {
    return sqrtf( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3] );
}
float vec4_lengthSq(const float* v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}
float vec4_lengthManhattan(const float* v) {
    return fabsf( v[0] ) + fabsf( v[1] ) + fabsf( v[2] ) + fabsf( v[3] );
}
void vec4_normalize(float* dst) {
    vec4_divideScalar(dst, vec4_length(dst));
}
void vec4_setLength(float* dst, float length) {
    vec4_multiplyScalar(dst, length / vec4_length(dst));
}
void vec4_lerp(float* dst, const float* v, float alpha) {
    dst[0] += ( v[0] - dst[0] ) * alpha;
    dst[1] += ( v[1] - dst[1] ) * alpha;
    dst[2] += ( v[2] - dst[2] ) * alpha;
    dst[3] += ( v[3] - dst[3] ) * alpha;
}
void vec4_lerpVectors(float* dst, const float* v1, const float* v2, float alpha) {
    vec4_subVectors(dst, v2, v1);
    vec4_multiplyScalar(dst, alpha);
    vec4_add(dst, v1);
}
bool vec4_equals(const float* a, const float* b) {
    return 0 == memcmp(a, b, sizeof(float) * 4);
}
void vec4_fromArray(float* dst, const float* array, int offset) {
    array += offset;
    memcpy(dst, array, sizeof(float) * 4);
}
void vec4_toArray(const float* dst, float* array, int offset) {
    array += offset;
    memcpy(array, dst, sizeof(float) * 4);
}
