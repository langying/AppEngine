//
//  AEMath.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/15.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEMath.hpp"
#include "AEEuler.hpp"
#include "AEMatrix3.hpp"
#include "AEMatrix4.hpp"
#include "AEVector3.hpp"
#include "AEVector4.hpp"
#include "AEQuaternion.hpp"

int eule_setFromRotationMatrix(duk_context* ctx) {
    float* e = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    const char* order = duk_get_string(ctx, 2);
    eule_setFromRotationMatrix(e, m, order);
    return 0;
}
int eule_setFromQuaternion(duk_context* ctx) {
    float* e = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* q = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    const char* order = duk_get_string(ctx, 2);
    eule_setFromQuaternion(e, q, order);
    return 0;
}
int eule_reorder(duk_context* ctx) {
    float* e = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    const char* order = duk_get_string(ctx, 1);
    const char* newer = duk_get_string(ctx, 2);
    eule_reorder(e, order, newer);
    return 0;
}

int mat3_identity(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    mat3_identity((float*)m);
    return 0;
}
int mat3_setFromMatrix4(duk_context* ctx) {
    float* m3 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m4 = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat3_setFromMatrix4(m3, m4);
    return 0;
}
int mat3_applyToVector3Array(duk_context* ctx) {
    void*  matrix = duk_get_buffer_data(ctx, 0, nullptr);
    void*  array  = duk_get_buffer_data(ctx, 1, nullptr);
    int    offset = duk_get_int(ctx, 2);
    int    length = duk_get_int(ctx, 3);
    mat3_applyToVector3Array((float*)matrix, (float*)array, offset, length);
    return 0;
}
int mat3_multiplyScalar(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    mat3_multiplyScalar((float*)m, s);
    return 0;
}
int mat3_determinant(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, mat3_determinant(m));
    return 1;
}
int mat3_getInverse(duk_context* ctx) {
    float* dst = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* src = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat3_getInverse(dst, src);
    return 0;
}
int mat3_transpose(duk_context* ctx) {
    float* m3 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    mat3_transpose(m3);
    return 0;
}
int mat3_getNormalMatrix(duk_context* ctx) {
    float* m3 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m4 = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat3_getNormalMatrix(m3, m4);
    return 0;
}
int mat3_transposeIntoArray(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat3_transposeIntoArray(a, b);
    return 0;
}
int mat3_fromArray(duk_context* ctx) {
    void* matrix = duk_get_buffer_data(ctx, 0, nullptr);
    void* array  = duk_get_buffer_data(ctx, 1, nullptr);
    GLint offset = duk_get_number(ctx, 2);
    mat3_fromArray((float*)matrix, (float*)array, offset);
    return 0;
}
int mat3_toArray(duk_context* ctx) {
    void* matrix = duk_get_buffer_data(ctx, 0, nullptr);
    void* array  = duk_get_buffer_data(ctx, 1, nullptr);
    GLint offset = duk_get_number(ctx, 2);
    mat3_toArray((float*)matrix, (float*)array, offset);
    return 0;
}

int mat4_identity(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    mat4_identity(m);
    return 0;
}
int mat4_extractRotation(duk_context* ctx) {
    float* dst = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* src = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat4_extractRotation(dst, src);
    return 0;
}
int mat4_makeRotationFromEuler(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* e = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    const char* order = duk_get_string(ctx, 2);
    mat4_makeRotationFromEuler(m, e, order);
    return 0;
}
int mat4_makeRotationFromQuaternion(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* q = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat4_makeRotationFromQuaternion(m, q);
    return 0;
}
int mat4_lookAt(duk_context* ctx) {
    float* mat = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* eye = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* tgt = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    float* up  = (float*) duk_get_buffer_data(ctx, 3, nullptr);
    mat4_lookAt(mat, eye, tgt, up);
    return 0;
}
int mat4_multiplyScalar(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    mat4_multiplyScalar((float*)m, s);
    return 0;
}
int mat4_multiplyMatrices(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    mat4_multiplyMatrices(m, a, b);
    return 0;
}
int mat4_applyToVector3Array(duk_context* ctx){
    void*  matrix = duk_get_buffer_data(ctx, 0, nullptr);
    void*  array  = duk_get_buffer_data(ctx, 1, nullptr);
    int    offset = duk_get_int(ctx, 2);
    int    length = duk_get_int(ctx, 3);
    mat4_applyToVector3Array((float*)matrix, (float*)array, offset, length);
    return 0;
}
int mat4_determinant(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, mat4_determinant(m));
    return 1;
}
int mat4_transpose(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    mat4_transpose(m);
    return 0;
}
int mat4_setPosition(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* v = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat4_setPosition(m, v);
    return 0;
}
int mat4_getInverse(duk_context* ctx) {
    float* dst = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* src = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat4_getInverse(dst, src);
    return 0;
}
int mat4_scale(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* v = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    mat4_scale(m, v);
    return 0;
}
int mat4_getMaxScaleOnAxis(duk_context* ctx) {
    float* m = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, mat4_getMaxScaleOnAxis(m));
    return 1;
}
int mat4_makeTranslation(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float x = duk_get_number(ctx, 1);
    float y = duk_get_number(ctx, 2);
    float z = duk_get_number(ctx, 3);
    mat4_makeTranslation((float*)m, x, y, z);
    return 0;
}
int mat4_makeRotationX(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float t = duk_get_number(ctx, 1);
    mat4_makeRotationX((float*)m, t);
    return 0;
}
int mat4_makeRotationY(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float t = duk_get_number(ctx, 1);
    mat4_makeRotationY((float*)m, t);
    return 0;
}
int mat4_makeRotationZ(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float t = duk_get_number(ctx, 1);
    mat4_makeRotationZ((float*)m, t);
    return 0;
}
int mat4_makeRotationAxis(duk_context* ctx) {
    void* mat4 = duk_get_buffer_data(ctx, 0, nullptr);
    void* axis = duk_get_buffer_data(ctx, 1, nullptr);
    float angl = duk_get_number(ctx, 2);
    mat4_makeRotationAxis((float*)mat4, (float*)axis, angl);
    return 0;
}
int mat4_makeScale(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float x = duk_get_number(ctx, 1);
    float y = duk_get_number(ctx, 2);
    float z = duk_get_number(ctx, 3);
    mat4_makeScale((float*)m, x, y, z);
    return 0;
}
int mat4_makeShear(duk_context* ctx) {
    void* m = duk_get_buffer_data(ctx, 0, nullptr);
    float x = duk_get_number(ctx, 1);
    float y = duk_get_number(ctx, 2);
    float z = duk_get_number(ctx, 3);
    mat4_makeShear((float*)m, x, y, z);
    return 0;
}
int mat4_compose(duk_context* ctx) {
    float* mat4 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* posi = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* quat = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    float* scal = (float*) duk_get_buffer_data(ctx, 3, nullptr);
    mat4_compose(mat4, posi, quat, scal);
    return 0;
}
int mat4_makePerspective(duk_context* ctx) {
    void* m4    = duk_get_buffer_data(ctx, 0, nullptr);
    float left  = duk_get_number(ctx, 1);
    float right = duk_get_number(ctx, 2);
    float top   = duk_get_number(ctx, 3);
    float bottom= duk_get_number(ctx, 4);
    float near  = duk_get_number(ctx, 5);
    float far   = duk_get_number(ctx, 6);
    mat4_makePerspective((float*)m4, left, right, top, bottom, near, far);
    return 0;
}
int mat4_makeOrthographic(duk_context* ctx) {
    void* m4    = duk_get_buffer_data(ctx, 0, nullptr);
    float left  = duk_get_number(ctx, 1);
    float right = duk_get_number(ctx, 2);
    float top   = duk_get_number(ctx, 3);
    float bottom= duk_get_number(ctx, 4);
    float near  = duk_get_number(ctx, 5);
    float far   = duk_get_number(ctx, 6);
    mat4_makeOrthographic((float*)m4, left, right, top, bottom, near, far);
    return 0;
}
int mat4_equals(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_boolean(ctx, mat4_equals(a, b));
    return 1;
}
int mat4_fromArray(duk_context* ctx) {
    void* matrix = duk_get_buffer_data(ctx, 0, nullptr);
    void* array  = duk_get_buffer_data(ctx, 1, nullptr);
    float offset = duk_get_number(ctx, 2);
    mat4_fromArray((float*)matrix, (float*)array, offset);
    return 0;
}
int mat4_toArray(duk_context* ctx) {
    void* matrix = duk_get_buffer_data(ctx, 0, nullptr);
    void* array  = duk_get_buffer_data(ctx, 1, nullptr);
    float offset = duk_get_number(ctx, 2);
    mat4_toArray((float*)matrix, (float*)array, offset);
    return 0;
}

int quat_setFromEuler(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* e = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    const char* order = duk_get_string(ctx, 2);
    quat_setFromEuler(q, e, order);
    return 0;
}
int quat_setFromAxisAngle(duk_context* ctx) {
    void* quat = duk_get_buffer_data(ctx, 0, nullptr);
    void* axis = duk_get_buffer_data(ctx, 1, nullptr);
    float angl = duk_get_number(ctx, 2);
    quat_setFromAxisAngle((float*)quat, (float*)axis, angl);
    return 0;
}
int quat_setFromRotationMatrix(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    quat_setFromRotationMatrix(q, m);
    return 0;
}
int quat_setFromUnitVectors(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* f = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* t = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    quat_setFromUnitVectors(q, f, t);
    return 0;
}
int quat_inverse(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    quat_inverse(q);
    return 0;
}
int quat_conjugate(duk_context* ctx){
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    quat_conjugate(q);
    return 0;
}
int quat_dot(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, quat_dot(a, b));
    return 1;
}
int quat_length(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, quat_length(q));
    return 1;
}
int quat_lengthSq(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, quat_lengthSq(q));
    return 1;
}
int quat_normalize(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    quat_normalize(q);
    return 0;
}
int quat_multiplyQuaternions(duk_context* ctx) {
    float* q = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    multiplyQuaternions(q, a, b);
    return 0;
}
int quat_slerp(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    float t = duk_get_number(ctx, 2);
    quat_slerp((float*)a, (float*)b, t);
    return 0;
}
int quat_equals(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_boolean(ctx, quat_equals(a, b));
    return 1;
}
int quat_fromArray(duk_context* ctx) {
    void* quat   = duk_get_buffer_data(ctx, 0, nullptr);
    void* array  = duk_get_buffer_data(ctx, 1, nullptr);
    int   offset = duk_get_number(ctx, 2);
    quat_fromArray((float*)quat, (float*)array, offset);
    return 0;
}
int quat_toArray(duk_context* ctx) {
    void* quat   = duk_get_buffer_data(ctx, 0, nullptr);
    void* array  = duk_get_buffer_data(ctx, 1, nullptr);
    int   offset = duk_get_number(ctx, 2);
    quat_toArray((float*)quat, (float*)array, offset);
    return 0;
}

int vec3_add(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_add(a, b);
    return 0;
}
int vec3_addScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec3_addScalar((float*)v, s);
    return 0;
}
int vec3_addVectors(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_addVectors(v, a, b);
    return 0;
}
int vec3_addScaledVector(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    float s = duk_get_number(ctx, 2);
    vec3_addScaledVector((float*)a, (float*)b, s);
    return 0;
}
int vec3_sub(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_sub(a, b);
    return 0;
}
int vec3_subScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec3_subScalar((float*)v, s);
    return 0;
}
int vec3_subVectors(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_subVectors(v, a, b);
    return 0;
}
int vec3_multiply(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_multiply(a, b);
    return 0;
}
int vec3_multiplyScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec3_multiplyScalar((float*)v, s);
    return 0;
}
int vec3_multiplyVectors(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_multiplyVectors(v, a, b);
    return 0;
}
int vec3_applyEuler(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    const char* order = duk_get_string(ctx, 2);
    vec3_applyEuler(a, b, order);
    return 0;
}
int vec3_applyAxisAngle(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    float t = duk_get_number(ctx, 2);
    vec3_applyAxisAngle((float*)a, (float*)b, t);
    return 0;
}
int vec3_applyMatrix3(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_applyMatrix3(v, m);
    return 0;
}
int vec3_applyMatrix4(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_applyMatrix4(v, m);
    return 0;
}
int vec3_applyProjection(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_applyProjection(v, m);
    return 0;
}
int vec3_applyQuaternion(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* q = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_applyQuaternion(v, q);
    return 0;
}
int vec3_project(duk_context* ctx) {
    float* v3 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* pm = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* mw = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_project(v3, pm, mw);
    return 0;
}
int vec3_unproject(duk_context* ctx) {
    float* v3 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* mw = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* pm = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_project(v3, mw, pm);
    return 0;
}
int vec3_transformDirection(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_transformDirection(v, m);
    return 0;
}
int vec3_divide(duk_context* ctx) {
    float* v1 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* v2 = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_divide(v1, v2);
    return 0;
}
int vec3_divideScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec3_divideScalar((float*)v, s);
    return 0;
}
int vec3_min(duk_context* ctx) {
    float* v1 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* v2 = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_min(v1, v2);
    return 0;
}
int vec3_max(duk_context* ctx) {
    float* v1 = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* v2 = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_max(v1, v2);
    return 0;
}
int vec3_clamp(duk_context* ctx) {
    float* dst = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* min = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* max = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_clamp(dst, min, max);
    return 0;
}
int vec3_clampScalar(duk_context* ctx) {
    void* dst = duk_get_buffer_data(ctx, 0, nullptr);
    float min = duk_get_number(ctx, 1);
    float max = duk_get_number(ctx, 2);
    vec3_clampScalar((float*)dst, min, max);
    return 0;
}
int vec3_clampLength(duk_context* ctx) {
    void* dst = duk_get_buffer_data(ctx, 0, nullptr);
    float min = duk_get_number(ctx, 1);
    float max = duk_get_number(ctx, 2);
    vec3_clampLength((float*)dst, min, max);
    return 0;
}
int vec3_floor(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec3_floor(v);
    return 0;
}
int vec3_ceil(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec3_ceil(v);
    return 0;
}
int vec3_round(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec3_round(v);
    return 0;
}
int vec3_roundToZero(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec3_roundToZero(v);
    return 0;
}
int vec3_negate(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec3_negate(v);
    return 0;
}
int vec3_dot(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, vec3_dot(a, b));
    return 1;
}
int vec3_length(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, vec3_length(v));
    return 1;
}
int vec3_lengthSq(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, vec3_lengthSq(v));
    return 1;
}
int vec3_lengthManhattan(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, vec3_lengthManhattan(v));
    return 1;
}
int vec3_normalize(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec3_normalize(v);
    return 0;
}
int vec3_setLength(duk_context* ctx) {
    void* dst = duk_get_buffer_data(ctx, 0, nullptr);
    float len = duk_get_number(ctx, 1);
    vec3_setLength((float*)dst, len);
    return 0;
}
int vec3_lerp(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    float c = duk_get_number(ctx, 2);
    vec3_lerp((float*)a, (float*)b, c);
    return 0;
}
int vec3_lerpVectors(duk_context* ctx) {
    void* v0 = duk_get_buffer_data(ctx, 0, nullptr);
    void* v1 = duk_get_buffer_data(ctx, 1, nullptr);
    void* v2 = duk_get_buffer_data(ctx, 2, nullptr);
    float al = duk_get_number(ctx, 3);
    vec3_lerpVectors((float*)v0, (float*)v1, (float*)v2, al);
    return 0;
}
int vec3_cross(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_cross(a, b);
    return 0;
}
int vec3_crossVectors(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec3_crossVectors(v, a, b);
    return 0;
}
int vec3_projectOnVector(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_projectOnVector(a, b);
    return 0;
}
int vec3_projectOnPlane(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_projectOnPlane(a, b);
    return 0;
}
int vec3_reflect(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_reflect(a, b);
    return 0;
}
int vec3_angleTo(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, vec3_angleTo(a, b));
    return 1;
}
int vec3_distanceTo(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, vec3_distanceTo(a, b));
    return 1;
}
int vec3_distanceToSquared(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, vec3_distanceToSquared(a, b));
    return 1;
}
int vec3_distanceToManhattan(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, vec3_distanceToManhattan(a, b));
    return 1;
}
int vec3_setFromMatrixPosition(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_setFromMatrixPosition(v, m);
    return 0;
}
int vec3_setFromMatrixScale(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec3_setFromMatrixScale(v, m);
    return 0;
}
int vec3_setFromMatrixColumn(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    void* m = duk_get_buffer_data(ctx, 1, nullptr);
    int   i = duk_get_int(ctx, 2);
    vec3_setFromMatrixColumn((float*)v, (float*)m, i);
    return 0;
}
int vec3_equals(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_boolean(ctx, vec3_equals(a, b));
    return 1;
}
int vec3_fromArray(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    int   i = duk_get_int(ctx, 2);
    vec3_fromArray((float*)a, (float*)b, i);
    return 0;
}
int vec3_toArray(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    int   i = duk_get_int(ctx, 2);
    vec3_toArray((float*)a, (float*)b, i);
    return 0;
}

int vec4_add(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_add(a, b);
	return 0;
}
int vec4_addScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec4_addScalar((float*)v, s);
	return 0;
}
int vec4_addVectors(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec4_addVectors(v, a, b);
	return 0;
}
int vec4_addScaledVector(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    float s = duk_get_number(ctx, 2);
    vec4_addScaledVector((float*)a, (float*)b, s);
	return 0;
}
int vec4_sub(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_sub(a, b);
	return 0;
}
int vec4_subScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec4_subScalar((float*)v, s);
	return 0;
}
int vec4_subVectors(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec4_subVectors(v, a, b);
	return 0;
}
int vec4_multiplyScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec4_multiplyScalar((float*)v, s);
    return 0;
}
int vec4_applyMatrix4(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_applyMatrix4(v, m);
	return 0;
}
int vec4_divideScalar(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    float s = duk_get_number(ctx, 1);
    vec4_divideScalar((float*)v, s);
    return 0;
}
int vec4_setAxisAngleFromQuaternion(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* q = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_setAxisAngleFromQuaternion(v, q);
    return 0;
}
int vec4_setAxisAngleFromRotationMatrix(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* m = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_setAxisAngleFromRotationMatrix(v, m);
    return 0;
}
int vec4_min(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_min(a, b);
	return 0;
}
int vec4_max(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    vec4_max(a, b);
	return 0;
}
int vec4_clamp(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* a = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 2, nullptr);
    vec4_clamp(v, a, b);
	return 0;
}
int vec4_clampScalar(duk_context* ctx) {
    void* dst = duk_get_buffer_data(ctx, 0, nullptr);
    float min = duk_get_number(ctx, 1);
    float max = duk_get_number(ctx, 2);
    vec4_clampScalar((float*)dst, min, max);
	return 0;
}
int vec4_floor(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec4_floor(v);
	return 0;
}
int vec4_ceil(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec4_ceil(v);
    return 0;
}
int vec4_round(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec4_round(v);
    return 0;
}
int vec4_roundToZero(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec4_roundToZero(v);
    return 0;
}
int vec4_negate(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec4_negate(v);
    return 0;
}
int vec4_dot(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_number(ctx, vec4_dot(a, b));
	return 1;
}
int vec4_length(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, vec4_length(v));
    return 1;
}
int vec4_lengthSq(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, vec4_lengthSq(v));
    return 1;
}
int vec4_lengthManhattan(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    duk_push_number(ctx, vec4_lengthManhattan(v));
    return 1;
}
int vec4_normalize(duk_context* ctx) {
    float* v = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    vec4_normalize(v);
	return 0;
}
int vec4_setLength(duk_context* ctx) {
    void* dst = duk_get_buffer_data(ctx, 0, nullptr);
    float len = duk_get_number(ctx, 1);
    vec4_setLength((float*)dst, len);
	return 0;
}
int vec4_lerp(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    float s = duk_get_number(ctx, 2);
    vec4_lerp((float*)a, (float*)b, s);
	return 0;
}
int vec4_lerpVectors(duk_context* ctx) {
    void* v = duk_get_buffer_data(ctx, 0, nullptr);
    void* a = duk_get_buffer_data(ctx, 1, nullptr);
    void* b = duk_get_buffer_data(ctx, 2, nullptr);
    float s = duk_get_number(ctx, 3);
    vec4_lerpVectors((float*)v, (float*)a, (float*)b, s);
	return 0;
}
int vec4_equals(duk_context* ctx) {
    float* a = (float*) duk_get_buffer_data(ctx, 0, nullptr);
    float* b = (float*) duk_get_buffer_data(ctx, 1, nullptr);
    duk_push_boolean(ctx, vec4_equals(a, b));
	return 1;
}
int vec4_fromArray(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    int   i = duk_get_int(ctx, 2);
    vec4_fromArray((float*)a, (float*)b, i);
	return 0;
}
int vec4_toArray(duk_context* ctx) {
    void* a = duk_get_buffer_data(ctx, 0, nullptr);
    void* b = duk_get_buffer_data(ctx, 1, nullptr);
    int   i = duk_get_int(ctx, 2);
    vec4_toArray((float*)a, (float*)b, i);
    return 0;
}
