//
//  AEMath.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/1/15.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEMath_hpp
#define AEMath_hpp

#include "AEPlatform.hpp"

int eule_setFromRotationMatrix(duk_context* ctx);
int eule_setFromQuaternion(duk_context* ctx);
int eule_reorder(duk_context* ctx);

int mat3_identity(duk_context* ctx);
int mat3_setFromMatrix4(duk_context* ctx);
int mat3_applyToVector3Array(duk_context* ctx);
int mat3_multiplyScalar(duk_context* ctx);
int mat3_determinant(duk_context* ctx);
int mat3_getInverse(duk_context* ctx);
int mat3_transpose(duk_context* ctx);
int mat3_getNormalMatrix(duk_context* ctx);
int mat3_transposeIntoArray(duk_context* ctx);
int mat3_fromArray(duk_context* ctx);
int mat3_toArray(duk_context* ctx);

int mat4_identity(duk_context* ctx);
int mat4_extractRotation(duk_context* ctx);
int mat4_makeRotationFromEuler(duk_context* ctx);
int mat4_makeRotationFromQuaternion(duk_context* ctx);
int mat4_lookAt(duk_context* ctx);
int mat4_multiplyScalar(duk_context* ctx);
int mat4_multiplyMatrices(duk_context* ctx);
int mat4_applyToVector3Array(duk_context* ctx);
int mat4_determinant(duk_context* ctx);
int mat4_transpose(duk_context* ctx);
int mat4_setPosition(duk_context* ctx);
int mat4_getInverse(duk_context* ctx);
int mat4_scale(duk_context* ctx);
int mat4_getMaxScaleOnAxis(duk_context* ctx);
int mat4_makeTranslation(duk_context* ctx);
int mat4_makeRotationX(duk_context* ctx);
int mat4_makeRotationY(duk_context* ctx);
int mat4_makeRotationZ(duk_context* ctx);
int mat4_makeRotationAxis(duk_context* ctx);
int mat4_makeScale(duk_context* ctx);
int mat4_makeShear(duk_context* ctx);
int mat4_compose(duk_context* ctx);
int mat4_makePerspective(duk_context* ctx);
int mat4_makeOrthographic(duk_context* ctx);
int mat4_equals(duk_context* ctx);
int mat4_fromArray(duk_context* ctx);
int mat4_toArray(duk_context* ctx);

int quat_setFromEuler(duk_context* ctx);
int quat_setFromAxisAngle(duk_context* ctx);
int quat_setFromRotationMatrix(duk_context* ctx);
int quat_setFromUnitVectors(duk_context* ctx);
int quat_inverse(duk_context* ctx);
int quat_conjugate(duk_context* ctx);
int quat_dot(duk_context* ctx);
int quat_length(duk_context* ctx);
int quat_lengthSq(duk_context* ctx);
int quat_normalize(duk_context* ctx);
int quat_multiplyQuaternions(duk_context* ctx);
int quat_slerp(duk_context* ctx);
int quat_equals(duk_context* ctx);
int quat_fromArray(duk_context* ctx);
int quat_toArray(duk_context* ctx);

int vec3_add(duk_context* ctx);
int vec3_addScalar(duk_context* ctx);
int vec3_addVectors(duk_context* ctx);
int vec3_addScaledVector(duk_context* ctx);
int vec3_sub(duk_context* ctx);
int vec3_subScalar(duk_context* ctx);
int vec3_subVectors(duk_context* ctx);
int vec3_multiply(duk_context* ctx);
int vec3_multiplyScalar(duk_context* ctx);
int vec3_multiplyVectors(duk_context* ctx);
int vec3_applyEuler(duk_context* ctx);
int vec3_applyAxisAngle(duk_context* ctx);
int vec3_applyMatrix3(duk_context* ctx);
int vec3_applyMatrix4(duk_context* ctx);
int vec3_applyProjection(duk_context* ctx);
int vec3_applyQuaternion(duk_context* ctx);
int vec3_project(duk_context* ctx);
int vec3_unproject(duk_context* ctx);
int vec3_transformDirection(duk_context* ctx);
int vec3_divide(duk_context* ctx);
int vec3_divideScalar(duk_context* ctx);
int vec3_min(duk_context* ctx);
int vec3_max(duk_context* ctx);
int vec3_clamp(duk_context* ctx);
int vec3_clampScalar(duk_context* ctx);
int vec3_clampLength(duk_context* ctx);
int vec3_floor(duk_context* ctx);
int vec3_ceil(duk_context* ctx);
int vec3_round(duk_context* ctx);
int vec3_roundToZero(duk_context* ctx);
int vec3_negate(duk_context* ctx);
int vec3_dot(duk_context* ctx);
int vec3_length(duk_context* ctx);
int vec3_lengthSq(duk_context* ctx);
int vec3_lengthManhattan(duk_context* ctx);
int vec3_normalize(duk_context* ctx);
int vec3_setLength(duk_context* ctx);
int vec3_lerp(duk_context* ctx);
int vec3_lerpVectors(duk_context* ctx);
int vec3_cross(duk_context* ctx);
int vec3_crossVectors(duk_context* ctx);
int vec3_projectOnVector(duk_context* ctx);
int vec3_projectOnPlane(duk_context* ctx);
int vec3_reflect(duk_context* ctx);
int vec3_angleTo(duk_context* ctx);
int vec3_distanceTo(duk_context* ctx);
int vec3_distanceToSquared(duk_context* ctx);
int vec3_distanceToManhattan(duk_context* ctx);
int vec3_setFromMatrixPosition(duk_context* ctx);
int vec3_setFromMatrixScale(duk_context* ctx);
int vec3_setFromMatrixColumn(duk_context* ctx);
int vec3_equals(duk_context* ctx);
int vec3_fromArray(duk_context* ctx);
int vec3_toArray(duk_context* ctx);

int vec4_set(duk_context* ctx);
int vec4_setScalar(duk_context* ctx);
int vec4_copy(duk_context* ctx);
int vec4_add(duk_context* ctx);
int vec4_addScalar(duk_context* ctx);
int vec4_addVectors(duk_context* ctx);
int vec4_addScaledVector(duk_context* ctx);
int vec4_sub(duk_context* ctx);
int vec4_subScalar(duk_context* ctx);
int vec4_subVectors(duk_context* ctx);
int vec4_multiplyScalar(duk_context* ctx);
int vec4_applyMatrix4(duk_context* ctx);
int vec4_divideScalar(duk_context* ctx);
int vec4_setAxisAngleFromQuaternion(duk_context* ctx);
int vec4_setAxisAngleFromRotationMatrix(duk_context* ctx);
int vec4_min(duk_context* ctx);
int vec4_max(duk_context* ctx);
int vec4_clamp(duk_context* ctx);
int vec4_clampScalar(duk_context* ctx);
int vec4_floor(duk_context* ctx);
int vec4_ceil(duk_context* ctx);
int vec4_round(duk_context* ctx);
int vec4_roundToZero(duk_context* ctx);
int vec4_negate(duk_context* ctx);
int vec4_dot(duk_context* ctx);
int vec4_length(duk_context* ctx);
int vec4_lengthSq(duk_context* ctx);
int vec4_lengthManhattan(duk_context* ctx);
int vec4_normalize(duk_context* ctx);
int vec4_setLength(duk_context* ctx);
int vec4_lerp(duk_context* ctx);
int vec4_lerpVectors(duk_context* ctx);
int vec4_equals(duk_context* ctx);
int vec4_fromArray(duk_context* ctx);
int vec4_toArray(duk_context* ctx);

#endif /* AEMath_hpp */
