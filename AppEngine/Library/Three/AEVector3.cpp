//
//  AEVector3.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include <cmath>
#include "AEVector3.hpp"
#include "AEMatrix4.hpp"
#include "AEQuaternion.hpp"

void vec3_set(float* v, float x, float y, float z) {
    v[0] = x;
    v[1] = y;
    v[2] = z;
}
void vec3_setScalar(float* v, float scalar) {
    v[0] = scalar;
    v[1] = scalar;
    v[2] = scalar;
}
void vec3_copy(float* dst, const float* src) {
    memcpy(dst, src, sizeof(float) * 3);
}
void vec3_add(float* dst, const float* src) {
    dst[0] += src[0];
    dst[1] += src[1];
    dst[2] += src[2];
}
void vec3_addScalar(float* v, float s){
    v[0] += s;
    v[1] += s;
    v[2] += s;
}
void vec3_addVectors(float* v, const float* a, const float* b) {
    v[0] = a[0] + b[0];
    v[1] = a[1] + b[1];
    v[2] = a[2] + b[2];
}
void vec3_addScaledVector(float* dst, const float* src, float s) {
    dst[0] += src[0] * s;
    dst[1] += src[1] * s;
    dst[2] += src[2] * s;
}
void vec3_sub(float* dst, const float* src) {
    dst[0] -= src[0];
    dst[1] -= src[1];
    dst[2] -= src[2];
}
void vec3_subScalar(float* dst, float s) {
    dst[0] -= s;
    dst[1] -= s;
    dst[2] -= s;
}
void vec3_subVectors(float* v, const float* a, const float* b) {
    v[0] = a[0] - b[0];
    v[1] = a[1] - b[1];
    v[2] = a[2] - b[2];
}
void vec3_multiply(float* dst, const float* src) {
    dst[0] *= src[0];
    dst[1] *= src[1];
    dst[2] *= src[2];
}
void vec3_multiplyScalar(float* dst, float scalar) {
    if (std::isfinite(scalar)) {
        dst[0] *= scalar;
        dst[1] *= scalar;
        dst[2] *= scalar;
    } else {
        dst[0] = 0;
        dst[1] = 0;
        dst[2] = 0;
    }
}
void vec3_multiplyVectors(float* dst, const float* a, const float* b) {
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
}
void vec3_applyEuler(float* dst, const float* euler, std::string order) {
    static float quaternion[4];
    quat_setFromEuler(quaternion, euler, order);
    vec3_applyQuaternion(dst, quaternion);
}
void vec3_applyAxisAngle(float* dst, const float* axis, float angle) {
    static float quaternion[4];
    quat_setFromAxisAngle(quaternion, axis, angle);
    vec3_applyQuaternion(dst, quaternion);
}
void vec3_applyMatrix3(float* dst, const float* e) {
    float x = dst[0], y = dst[1], z = dst[2];
    dst[0] = e[ 0 ] * x + e[ 3 ] * y + e[ 6 ] * z;
    dst[1] = e[ 1 ] * x + e[ 4 ] * y + e[ 7 ] * z;
    dst[2] = e[ 2 ] * x + e[ 5 ] * y + e[ 8 ] * z;
}
void vec3_applyMatrix4(float* dst, const float* e) {
    float x = dst[0], y = dst[1], z = dst[2];
    dst[0] = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z + e[ 12 ];
    dst[1] = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z + e[ 13 ];
    dst[2] = e[ 2 ] * x + e[ 6 ] * y + e[10 ] * z + e[ 14 ];
}
void vec3_applyProjection(float* dst, const float* e) {
    float x = dst[0], y = dst[1], z = dst[2];
    float d = 1 / ( e[ 3 ] * x + e[ 7 ] * y + e[ 11 ] * z + e[ 15 ] ); // perspective divide
    
    dst[0] = ( e[ 0 ] * x + e[ 4 ] * y + e[ 8 ]  * z + e[ 12 ] ) * d;
    dst[1] = ( e[ 1 ] * x + e[ 5 ] * y + e[ 9 ]  * z + e[ 13 ] ) * d;
    dst[2] = ( e[ 2 ] * x + e[ 6 ] * y + e[ 10 ] * z + e[ 14 ] ) * d;
}
void vec3_applyQuaternion(float* v, const float* q) {
    float x  = v[0], y  = v[1], z  = v[2];
    float qx = q[0], qy = q[1], qz = q[2], qw = q[3];
    
    // calculate quat * vector
    float ix =  qw * x + qy * z - qz * y;
    float iy =  qw * y + qz * x - qx * z;
    float iz =  qw * z + qx * y - qy * x;
    float iw = -qx * x - qy * y - qz * z;
    
    // calculate result * inverse quat
    v[0] = ix * qw + iw * - qx + iy * - qz - iz * - qy;
    v[1] = iy * qw + iw * - qy + iz * - qx - ix * - qz;
    v[2] = iz * qw + iw * - qz + ix * - qy - iy * - qx;
}
void vec3_project(float* dst, const float* projectionMatrix, const float* matrixWorld) {
    static float matrix[16];
    mat4_getInverse(matrix, matrixWorld);
    mat4_multiplyMatrices(matrix, projectionMatrix, matrix);
    vec3_applyProjection(dst, matrix);
}
void vec3_unproject(float* dst, const float* matrixWorld, const float* projectionMatrix) {
    static float matrix[16];
    mat4_getInverse(matrix, projectionMatrix);
    mat4_multiplyMatrices(matrix, matrixWorld, matrix);
    vec3_applyProjection(dst, matrix);
}
void vec3_transformDirection(float* dst, const float* e) {
    float x = dst[0], y = dst[1], z = dst[2];
    dst[0] = e[ 0 ] * x + e[ 4 ] * y + e[ 8 ] * z;
    dst[1] = e[ 1 ] * x + e[ 5 ] * y + e[ 9 ] * z;
    dst[2] = e[ 2 ] * x + e[ 6 ] * y + e[10 ] * z;
    vec3_normalize(dst);
}
void vec3_divide(float* dst, const float* v) {
    dst[0] /= v[0];
    dst[1] /= v[1];
    dst[2] /= v[2];
}
void vec3_divideScalar(float* dst, float scalar) {
    vec3_multiplyScalar(dst, 1 / scalar);
}
void vec3_min(float* dst, const float* v) {
    dst[0] = fminf( dst[0], v[0] );
    dst[1] = fminf( dst[1], v[1] );
    dst[2] = fminf( dst[2], v[2] );
}
void vec3_max(float* dst, const float* v) {
    dst[0] = fmaxf( dst[0], v[0] );
    dst[1] = fmaxf( dst[1], v[1] );
    dst[2] = fmaxf( dst[2], v[2] );
}
void vec3_clamp(float* dst, const float* min, const float* max) {
    dst[0] = fmaxf( min[0], fminf( max[0], dst[0] ) );
    dst[1] = fmaxf( min[1], fminf( max[1], dst[1] ) );
    dst[2] = fmaxf( min[2], fminf( max[2], dst[2] ) );
}
void vec3_clampScalar(float* dst, float minVal, float maxVal) {
    static float min[3], max[3];
    vec3_set(min, minVal, minVal, minVal);
    vec3_set(max, maxVal, maxVal, maxVal);
    vec3_clamp(dst, min, max);
}
void vec3_clampLength(float* dst, float min, float max) {
    float length = vec3_length(dst);
    vec3_multiplyScalar(dst, fmaxf(min, fminf(max, length)) / length);
}
void vec3_floor(float* dst) {
    dst[0] = floorf( dst[0] );
    dst[1] = floorf( dst[1] );
    dst[2] = floorf( dst[2] );
}
void vec3_ceil(float* dst) {
    dst[0] = ceilf( dst[0] );
    dst[1] = ceilf( dst[1] );
    dst[2] = ceilf( dst[2] );
}
void vec3_round(float* dst) {
    dst[0] = roundf( dst[0] );
    dst[1] = roundf( dst[1] );
    dst[2] = roundf( dst[2] );
}
void vec3_roundToZero(float* dst) {
    dst[0] = ( dst[0] < 0 ) ? ceilf( dst[0] ) : floorf( dst[0] );
    dst[1] = ( dst[1] < 0 ) ? ceilf( dst[1] ) : floorf( dst[1] );
    dst[2] = ( dst[2] < 0 ) ? ceilf( dst[2] ) : floorf( dst[2] );
}
void vec3_negate(float* dst) {
    dst[0] = - dst[0];
    dst[1] = - dst[1];
    dst[2] = - dst[2];
}
float vec3_dot(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
float vec3_length(const float* v) {
    float x = v[0], y = v[1], z = v[2];
    return sqrtf(x*x + y*y + z*z);
}
float vec3_lengthSq(const float* v) {
    float x = v[0], y = v[1], z = v[2];
    return x*x + y*y + z*z;
}
float vec3_lengthManhattan(const float* v) {
    return fabsf( v[0] ) + fabsf( v[1] ) + fabsf( v[2] );
}
void vec3_normalize(float* v) {
    vec3_divideScalar(v, vec3_length(v));
}
void vec3_setLength(float* dst, float length) {
    vec3_multiplyScalar(dst, length / vec3_length(dst));
}
void vec3_lerp(float* dst, const float* v, float alpha) {
    dst[0] += ( v[0] - dst[0] ) * alpha;
    dst[1] += ( v[1] - dst[1] ) * alpha;
    dst[2] += ( v[2] - dst[2] ) * alpha;
}
void vec3_lerpVectors(float* dst, const float* v1, const float* v2, float alpha) {
    vec3_subVectors(dst, v2, v1);
    vec3_multiplyScalar(dst, alpha);
    vec3_add(dst, v1);
}
void vec3_cross(float* dst, const float* v) {
    float x = dst[0], y = dst[1], z = dst[2];
    
    dst[0] = y * v[2] - z * v[1];
    dst[1] = z * v[0] - x * v[2];
    dst[2] = x * v[1] - y * v[0];
}
void vec3_crossVectors(float* v, const float* a, const float* b) {
    float ax = a[0], ay = a[1], az = a[2];
    float bx = b[0], by = b[1], bz = b[2];
    v[0] = ay * bz - az * by;
    v[1] = az * bx - ax * bz;
    v[2] = ax * by - ay * bx;
}
void vec3_projectOnVector(float* dst, const float* vector) {
    float scalar = vec3_dot(vector, dst) / vec3_lengthSq(vector);
    vec3_copy(dst, vector);
    vec3_multiplyScalar(dst, scalar);
}
void vec3_projectOnPlane(float* dst, const float* planeNormal) {
    static float v1[3];
    vec3_copy(v1, dst);
    vec3_projectOnVector(v1, planeNormal);
    vec3_sub(dst, v1);
}
void vec3_reflect(float* dst, const float* normal) {
    static float v1[3];
    vec3_copy(v1, normal);
    vec3_multiplyScalar(v1, 2 * vec3_dot(dst, normal));
    vec3_sub(dst, v1);
}
float vec3_angleTo(const float* a, const float* b) {
    float theta = vec3_dot(a, b) / sqrtf( vec3_lengthSq(a) * vec3_lengthSq(b));
    return acosf(clampf(theta, -1, 1));
}
float vec3_distanceTo(const float* a, const float* b) {
    return sqrtf(vec3_distanceToSquared(a, b) );
}
float vec3_distanceToSquared(const float* a, const float* b) {
    float dx = a[0] - b[0], dy = a[1] - b[1], dz = a[2] - b[2];
    return dx * dx + dy * dy + dz * dz;
}
float vec3_distanceToManhattan(const float* a, const float* b) {
    return fabsf(a[0] - b[0]) + fabsf(a[1] - b[2]) + fabsf(a[2] - b[2]);
}
void vec3_setFromMatrixPosition(float* dst, const float* m4) {
    vec3_setFromMatrixColumn(dst, m4, 3);
}
void vec3_setFromMatrixScale(float* dst, const float* m) {
    vec3_setFromMatrixColumn(dst, m, 0);
    float sx = vec3_length(dst);
    vec3_setFromMatrixColumn(dst, m, 1);
    float sy = vec3_length(dst);
    vec3_setFromMatrixColumn(dst, m, 2);
    float sz = vec3_length(dst);
    
    dst[0] = sx;
    dst[1] = sy;
    dst[2] = sz;
}
void vec3_setFromMatrixColumn(float* dst, const float* m, int index) {
    vec3_fromArray(dst, m, index * 4);
}
bool vec3_equals(const float* a, const float* b) {
    return 0 == memcmp(a, b, sizeof(float) * 3);
}
void vec3_fromArray(float* dst, const float* array, int offset) {
    array += offset;
    memcpy(dst, array, sizeof(float) * 3);
}
void vec3_toArray(const float* v, float* array, int offset) {
    array += offset;
    memcpy(array, v, sizeof(float) * 3);
}

