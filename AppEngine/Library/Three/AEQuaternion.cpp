//
//  AEQuaternion.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEVector3.hpp"
#include "AEQuaternion.hpp"

void quat_set(float* q, float x, float y, float z, float w) {
    q[0] = x;
    q[1] = y;
    q[2] = z;
    q[3] = w;
}
void quat_copy(float* dst, const float* src) {
    memcpy(dst, src, sizeof(float) * 4);
}
void quat_setFromEuler(float* q, const float* e, std::string order) {
    float c1 = cosf( e[0] / 2 );
    float c2 = cosf( e[1] / 2 );
    float c3 = cosf( e[2] / 2 );
    float s1 = sinf( e[0] / 2 );
    float s2 = sinf( e[1] / 2 );
    float s3 = sinf( e[2] / 2 );
    
    if ( order == "XYZ" ) {
        
        q[0] = s1 * c2 * c3 + c1 * s2 * s3;
        q[1] = c1 * s2 * c3 - s1 * c2 * s3;
        q[2] = c1 * c2 * s3 + s1 * s2 * c3;
        q[3] = c1 * c2 * c3 - s1 * s2 * s3;
        
    } else if ( order == "YXZ" ) {
        
        q[0] = s1 * c2 * c3 + c1 * s2 * s3;
        q[1] = c1 * s2 * c3 - s1 * c2 * s3;
        q[2] = c1 * c2 * s3 - s1 * s2 * c3;
        q[3] = c1 * c2 * c3 + s1 * s2 * s3;
        
    } else if ( order == "ZXY" ) {
        
        q[0] = s1 * c2 * c3 - c1 * s2 * s3;
        q[1] = c1 * s2 * c3 + s1 * c2 * s3;
        q[2] = c1 * c2 * s3 + s1 * s2 * c3;
        q[3] = c1 * c2 * c3 - s1 * s2 * s3;
        
    } else if ( order == "ZYX" ) {
        
        q[0] = s1 * c2 * c3 - c1 * s2 * s3;
        q[1] = c1 * s2 * c3 + s1 * c2 * s3;
        q[2] = c1 * c2 * s3 - s1 * s2 * c3;
        q[3] = c1 * c2 * c3 + s1 * s2 * s3;
        
    } else if ( order == "YZX" ) {
        
        q[0] = s1 * c2 * c3 + c1 * s2 * s3;
        q[1] = c1 * s2 * c3 + s1 * c2 * s3;
        q[2] = c1 * c2 * s3 - s1 * s2 * c3;
        q[3] = c1 * c2 * c3 - s1 * s2 * s3;
        
    } else if ( order == "XZY" ) {
        
        q[0] = s1 * c2 * c3 - c1 * s2 * s3;
        q[1] = c1 * s2 * c3 - s1 * c2 * s3;
        q[2] = c1 * c2 * s3 + s1 * s2 * c3;
        q[3] = c1 * c2 * c3 + s1 * s2 * s3;
        
    }
}
void quat_setFromAxisAngle(float* q, const float* axis, float angle) {
    float halfAngle = angle / 2;
    float s = sinf( halfAngle );
    
    q[0] = axis[0] * s;
    q[1] = axis[1] * s;
    q[2] = axis[2] * s;
    q[3] = cosf( halfAngle );
}
void quat_setFromRotationMatrix(float* q, const float* te) {
    float m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ];
    float m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ];
    float m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[ 10 ];
    
    float trace = m11 + m22 + m33;
    float s;
    
    if ( trace > 0 ) {
        
        s = 0.5 / sqrtf( trace + 1.0 );
        
        q[3] = 0.25 / s;
        q[0] = ( m32 - m23 ) * s;
        q[1] = ( m13 - m31 ) * s;
        q[2] = ( m21 - m12 ) * s;
        
    } else if ( m11 > m22 && m11 > m33 ) {
        
        s = 2.0 * sqrtf( 1.0 + m11 - m22 - m33 );
        
        q[3] = ( m32 - m23 ) / s;
        q[0] = 0.25 * s;
        q[1] = ( m12 + m21 ) / s;
        q[2] = ( m13 + m31 ) / s;
        
    } else if ( m22 > m33 ) {
        
        s = 2.0 * sqrtf( 1.0 + m22 - m11 - m33 );
        
        q[3] = ( m13 - m31 ) / s;
        q[0] = ( m12 + m21 ) / s;
        q[1] = 0.25 * s;
        q[2] = ( m23 + m32 ) / s;
        
    } else {
        
        s = 2.0 * sqrtf( 1.0 + m33 - m11 - m22 );
        
        q[3] = ( m21 - m12 ) / s;
        q[0] = ( m13 + m31 ) / s;
        q[1] = ( m23 + m32 ) / s;
        q[2] = 0.25 * s;
        
    }
}
void quat_setFromUnitVectors(float* q, const float* vFrom, const float* vTo) {
    static float v1[3];
    static float EPS = 0.000001;
    float r = vec3_dot(vFrom, vTo) + 1;
    
    if ( r < EPS ) {

        r = 0;
        
        if ( fabsf( vFrom[0] ) > fabsf( vFrom[2] ) ) {
            
            vec3_set(v1, -vFrom[1], vFrom[0], 0 );
            
        } else {
            
            vec3_set(v1, 0, -vFrom[2], vFrom[1] );
            
        }
        
    } else {
        
        vec3_crossVectors(v1, vFrom, vTo );
        
    }
    
    q[0] = v1[0];
    q[1] = v1[1];
    q[2] = v1[2];
    q[3] = r;
    
    quat_normalize(q);
}
void quat_inverse(float* q) {
    quat_conjugate(q);
    quat_normalize(q);
}
void quat_conjugate(float* q) {
    q[0] *= -1;
    q[1] *= -1;
    q[2] *= -1;
}
float quat_dot(const float* q, const float* v) {
    return q[0]*v[0] + q[1]*v[1] + q[2]*v[2] + q[3]*v[3];
}
float quat_length(const float* q) {
    return sqrtf(q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3]);
}
float quat_lengthSq(const float* q) {
    return q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];
}
void quat_normalize(float* q) {
    float l = quat_length(q);
    
    if ( l == 0 ) {
        
        q[0] = 0;
        q[1] = 0;
        q[2] = 0;
        q[3] = 1;
        
    } else {
        
        l = 1 / l;
        
        q[0] = q[0] * l;
        q[1] = q[1] * l;
        q[2] = q[2] * l;
        q[3] = q[3] * l;
        
    }
}
void multiplyQuaternions(float* q, const float* a, const float* b) {
    float qax = a[0], qay = a[1], qaz = a[2], qaw = a[3];
    float qbx = b[0], qby = b[1], qbz = b[2], qbw = b[3];
    
    q[0] = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
    q[1] = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
    q[2] = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
    q[3] = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;
}
void quat_slerp(float* q, const float* qb, float t) {
    if ( t == 0 ) {
        return;
    }
    if ( t == 1 ) {
        quat_copy(q, qb);
        return;
    }
    
    float x = q[0], y = q[1], z = q[2], w = q[3];
    
    // http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
    
    float cosHalfTheta = w * qb[3] + x * qb[0] + y * qb[1] + z * qb[2];
    
    if ( cosHalfTheta < 0 ) {
        
        q[3] = - qb[3];
        q[0] = - qb[0];
        q[1] = - qb[1];
        q[2] = - qb[2];
        
        cosHalfTheta = - cosHalfTheta;
        
    } else {
        
        quat_copy(q, qb);
        
    }
    
    if ( cosHalfTheta >= 1.0 ) {
        
        q[3] = w;
        q[0] = x;
        q[1] = y;
        q[2] = z;
        
        return;
    }
    
    float sinHalfTheta = sqrtf( 1.0 - cosHalfTheta * cosHalfTheta );
    
    if ( fabsf( sinHalfTheta ) < 0.001 ) {
        
        q[3] = 0.5 * ( w + q[3] );
        q[0] = 0.5 * ( x + q[0] );
        q[1] = 0.5 * ( y + q[1] );
        q[2] = 0.5 * ( z + q[2] );
        
        return;
        
    }
    
    float halfTheta = atan2f( sinHalfTheta, cosHalfTheta );
    float ratioA = sinf( ( 1 - t ) * halfTheta ) / sinHalfTheta;
    float ratioB = sinf( t * halfTheta ) / sinHalfTheta;
    
    q[3] = ( w * ratioA + q[3] * ratioB );
    q[0] = ( x * ratioA + q[0] * ratioB );
    q[1] = ( y * ratioA + q[1] * ratioB );
    q[2] = ( z * ratioA + q[2] * ratioB );
}
bool quat_equals(const float* a, const float* b) {
    return 0 == memcmp(a, b, sizeof(float) * 4);
}
void quat_fromArray(float* q, const float* array, int offset) {
    array += offset;
    memcpy(q, array, sizeof(float) * 4);
}
void quat_toArray(const float* q, float* array, int offset) {
    array += offset;
    memcpy(array, q, sizeof(float) * 4);
}

