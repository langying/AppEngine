//
//  AEMatrix4.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/6.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEVector3.hpp"
#include "AEMatrix4.hpp"

void mat4_set(float* m,
              float n11, float n12, float n13, float n14,
              float n21, float n22, float n23, float n24,
              float n31, float n32, float n33, float n34,
              float n41, float n42, float n43, float n44) {
    m[ 0 ] = n11; m[ 4 ] = n12; m[ 8 ] = n13; m[ 12 ] = n14;
    m[ 1 ] = n21; m[ 5 ] = n22; m[ 9 ] = n23; m[ 13 ] = n24;
    m[ 2 ] = n31; m[ 6 ] = n32; m[10 ] = n33; m[ 14 ] = n34;
    m[ 3 ] = n41; m[ 7 ] = n42; m[11 ] = n43; m[ 15 ] = n44;
}
void mat4_identity(float* m) {
    mat4_set(m,
             1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1);
}
void mat4_copy(float* dst, const float* src) {
    memcpy(dst, src, sizeof(float) * 16);
}


void mat4_extractRotation(float* te, const float* me) {
    float scaleX = 1 / vec3_length(me + 4 * 0);
    float scaleY = 1 / vec3_length(me + 4 * 1);
    float scaleZ = 1 / vec3_length(me + 4 * 2);
    
    te[ 0 ] = me[ 0 ] * scaleX;
    te[ 1 ] = me[ 1 ] * scaleX;
    te[ 2 ] = me[ 2 ] * scaleX;
    
    te[ 4 ] = me[ 4 ] * scaleY;
    te[ 5 ] = me[ 5 ] * scaleY;
    te[ 6 ] = me[ 6 ] * scaleY;
    
    te[ 8 ] = me[ 8 ] * scaleZ;
    te[ 9 ] = me[ 9 ] * scaleZ;
    te[10 ] = me[10 ] * scaleZ;
}
void  mat4_makeRotationFromEuler(float* te, const float* euler, std::string order) {
    float x = euler[0], y = euler[1], z = euler[2];
    float a = cosf( x ), b = sinf( x );
    float c = cosf( y ), d = sinf( y );
    float e = cosf( z ), f = sinf( z );
    
    if (order == "XYZ" ) {
        
        float ae = a * e, af = a * f, be = b * e, bf = b * f;
        
        te[ 0 ] = c * e;
        te[ 4 ] = - c * f;
        te[ 8 ] = d;
        
        te[ 1 ] = af + be * d;
        te[ 5 ] = ae - bf * d;
        te[ 9 ] = - b * c;
        
        te[ 2 ] = bf - ae * d;
        te[ 6 ] = be + af * d;
        te[10 ] = a * c;
        
    } else if ( order == "YXZ" ) {
        
        float ce = c * e, cf = c * f, de = d * e, df = d * f;
        
        te[ 0 ] = ce + df * b;
        te[ 4 ] = de * b - cf;
        te[ 8 ] = a * d;
        
        te[ 1 ] = a * f;
        te[ 5 ] = a * e;
        te[ 9 ] = - b;
        
        te[ 2 ] = cf * b - de;
        te[ 6 ] = df + ce * b;
        te[10 ] = a * c;
        
    } else if ( order == "ZXY" ) {
        
        float ce = c * e, cf = c * f, de = d * e, df = d * f;
        
        te[ 0 ] = ce - df * b;
        te[ 4 ] = - a * f;
        te[ 8 ] = de + cf * b;
        
        te[ 1 ] = cf + de * b;
        te[ 5 ] = a * e;
        te[ 9 ] = df - ce * b;
        
        te[ 2 ] = - a * d;
        te[ 6 ] = b;
        te[10 ] = a * c;
        
    } else if ( order == "ZYX" ) {
        
        float ae = a * e, af = a * f, be = b * e, bf = b * f;
        
        te[ 0 ] = c * e;
        te[ 4 ] = be * d - af;
        te[ 8 ] = ae * d + bf;
        
        te[ 1 ] = c * f;
        te[ 5 ] = bf * d + ae;
        te[ 9 ] = af * d - be;
        
        te[ 2 ] = - d;
        te[ 6 ] = b * c;
        te[10 ] = a * c;
        
    } else if ( order == "YZX" ) {
        
        float ac = a * c, ad = a * d, bc = b * c, bd = b * d;
        
        te[ 0 ] = c * e;
        te[ 4 ] = bd - ac * f;
        te[ 8 ] = bc * f + ad;
        
        te[ 1 ] = f;
        te[ 5 ] = a * e;
        te[ 9 ] = - b * e;
        
        te[ 2 ] = - d * e;
        te[ 6 ] = ad * f + bc;
        te[10 ] = ac - bd * f;
        
    } else if ( order == "XZY" ) {
        
        float ac = a * c, ad = a * d, bc = b * c, bd = b * d;
        
        te[ 0 ] = c * e;
        te[ 4 ] = - f;
        te[ 8 ] = d * e;
        
        te[ 1 ] = ac * f + bd;
        te[ 5 ] = a * e;
        te[ 9 ] = ad * f - bc;
        
        te[ 2 ] = bc * f - ad;
        te[ 6 ] = b * e;
        te[10 ] = bd * f + ac;
        
    }
    
    // last column
    te[  3 ] = 0;
    te[  7 ] = 0;
    te[ 11 ] = 0;
    
    // bottom row
    te[ 12 ] = 0;
    te[ 13 ] = 0;
    te[ 14 ] = 0;
    te[ 15 ] = 1;
}
void mat4_makeRotationFromQuaternion(float* te, const float* q) {
    float x = q[0], y = q[1], z = q[2], w = q[3];
    float x2 = x + x,  y2 = y + y,  z2 = z + z;
    float xx = x * x2, xy = x * y2, xz = x * z2;
    float yy = y * y2, yz = y * z2, zz = z * z2;
    float wx = w * x2, wy = w * y2, wz = w * z2;
    
    te[ 0 ] = 1 - ( yy + zz );
    te[ 4 ] = xy - wz;
    te[ 8 ] = xz + wy;
    
    te[ 1 ] = xy + wz;
    te[ 5 ] = 1 - ( xx + zz );
    te[ 9 ] = yz - wx;
    
    te[ 2 ] = xz - wy;
    te[ 6 ] = yz + wx;
    te[10 ] = 1 - ( xx + yy );
    
    // last column
    te[ 3 ] = 0;
    te[ 7 ] = 0;
    te[11 ] = 0;
    
    // bottom row
    te[ 12 ] = 0;
    te[ 13 ] = 0;
    te[ 14 ] = 0;
    te[ 15 ] = 1;
}
void mat4_lookAt(float* te, float* eye, float* target, float* up) {
    static float x[3], y[3], z[3];
    vec3_subVectors(z, eye, target);
    vec3_normalize(z);
    if (vec3_lengthSq(z) == 0) {
        z[2] = 1;
    }
    
    vec3_crossVectors(x, up, z);
    vec3_normalize(x);
    if (vec3_lengthSq(x) == 0) {
        z[2] += 0.0001;
        vec3_crossVectors(x, up, z);
        vec3_normalize(x);
    }
    
    vec3_crossVectors(y, z, x);
    te[ 0 ] = x[0]; te[ 4 ] = y[0]; te[ 8 ] = z[0];
    te[ 1 ] = x[1]; te[ 5 ] = y[1]; te[ 9 ] = z[1];
    te[ 2 ] = x[2]; te[ 6 ] = y[2]; te[10 ] = z[2];
}
void mat4_multiplyScalar(float* te, float s) {
    te[ 0 ] *= s; te[ 4 ] *= s; te[ 8 ] *= s; te[ 12 ] *= s;
    te[ 1 ] *= s; te[ 5 ] *= s; te[ 9 ] *= s; te[ 13 ] *= s;
    te[ 2 ] *= s; te[ 6 ] *= s; te[10 ] *= s; te[ 14 ] *= s;
    te[ 3 ] *= s; te[ 7 ] *= s; te[11 ] *= s; te[ 15 ] *= s;
}
void mat4_multiplyMatrices(float* te, const float* ae, const float* be) {
    float a11 = ae[ 0 ], a12 = ae[ 4 ], a13 = ae[ 8 ], a14 = ae[ 12 ];
    float a21 = ae[ 1 ], a22 = ae[ 5 ], a23 = ae[ 9 ], a24 = ae[ 13 ];
    float a31 = ae[ 2 ], a32 = ae[ 6 ], a33 = ae[10 ], a34 = ae[ 14 ];
    float a41 = ae[ 3 ], a42 = ae[ 7 ], a43 = ae[11 ], a44 = ae[ 15 ];
    
    float b11 = be[ 0 ], b12 = be[ 4 ], b13 = be[ 8 ], b14 = be[ 12 ];
    float b21 = be[ 1 ], b22 = be[ 5 ], b23 = be[ 9 ], b24 = be[ 13 ];
    float b31 = be[ 2 ], b32 = be[ 6 ], b33 = be[10 ], b34 = be[ 14 ];
    float b41 = be[ 3 ], b42 = be[ 7 ], b43 = be[11 ], b44 = be[ 15 ];
    
    te[ 0 ] = a11 * b11 + a12 * b21 + a13 * b31 + a14 * b41;
    te[ 4 ] = a11 * b12 + a12 * b22 + a13 * b32 + a14 * b42;
    te[ 8 ] = a11 * b13 + a12 * b23 + a13 * b33 + a14 * b43;
    te[12 ] = a11 * b14 + a12 * b24 + a13 * b34 + a14 * b44;
    
    te[ 1 ] = a21 * b11 + a22 * b21 + a23 * b31 + a24 * b41;
    te[ 5 ] = a21 * b12 + a22 * b22 + a23 * b32 + a24 * b42;
    te[ 9 ] = a21 * b13 + a22 * b23 + a23 * b33 + a24 * b43;
    te[13 ] = a21 * b14 + a22 * b24 + a23 * b34 + a24 * b44;
    
    te[ 2 ] = a31 * b11 + a32 * b21 + a33 * b31 + a34 * b41;
    te[ 6 ] = a31 * b12 + a32 * b22 + a33 * b32 + a34 * b42;
    te[10 ] = a31 * b13 + a32 * b23 + a33 * b33 + a34 * b43;
    te[14 ] = a31 * b14 + a32 * b24 + a33 * b34 + a34 * b44;
    
    te[ 3 ] = a41 * b11 + a42 * b21 + a43 * b31 + a44 * b41;
    te[ 7 ] = a41 * b12 + a42 * b22 + a43 * b32 + a44 * b42;
    te[11 ] = a41 * b13 + a42 * b23 + a43 * b33 + a44 * b43;
    te[15 ] = a41 * b14 + a42 * b24 + a43 * b34 + a44 * b44;
}
void mat4_applyToVector3Array(const float* m, float* array, int offset, int length) {
    static float v1[3];
    for (int i = 0, j = offset; i < length; i += 3, j += 3) {
        vec3_fromArray(v1, array, j);
        vec3_applyMatrix4(v1, m);
        vec3_toArray(v1, array, j);
    }
}
float mat4_determinant(const float* te) {
    float n11 = te[ 0 ], n12 = te[ 4 ], n13 = te[ 8 ], n14 = te[ 12 ];
    float n21 = te[ 1 ], n22 = te[ 5 ], n23 = te[ 9 ], n24 = te[ 13 ];
    float n31 = te[ 2 ], n32 = te[ 6 ], n33 = te[10 ], n34 = te[ 14 ];
    float n41 = te[ 3 ], n42 = te[ 7 ], n43 = te[11 ], n44 = te[ 15 ];

    return (
            n41 * (
                   + n14 * n23 * n32
                   - n13 * n24 * n32
                   - n14 * n22 * n33
                   + n12 * n24 * n33
                   + n13 * n22 * n34
                   - n12 * n23 * n34
                   ) +
            n42 * (
                   + n11 * n23 * n34
                   - n11 * n24 * n33
                   + n14 * n21 * n33
                   - n13 * n21 * n34
                   + n13 * n24 * n31
                   - n14 * n23 * n31
                   ) +
            n43 * (
                   + n11 * n24 * n32
                   - n11 * n22 * n34
                   - n14 * n21 * n32
                   + n12 * n21 * n34
                   + n14 * n22 * n31
                   - n12 * n24 * n31
                   ) +
            n44 * (
                   - n13 * n22 * n31
                   - n11 * n23 * n32
                   + n11 * n22 * n33
                   + n13 * n21 * n32
                   - n12 * n21 * n33
                   + n12 * n23 * n31
                   )
            );
}
void mat4_transpose(float* te) {
    float tmp;
    
    tmp = te[ 1 ]; te[ 1 ] = te[ 4 ]; te[ 4 ] = tmp;
    tmp = te[ 2 ]; te[ 2 ] = te[ 8 ]; te[ 8 ] = tmp;
    tmp = te[ 6 ]; te[ 6 ] = te[ 9 ]; te[ 9 ] = tmp;
    
    tmp = te[ 3 ]; te[ 3 ] = te[ 12 ]; te[ 12 ] = tmp;
    tmp = te[ 7 ]; te[ 7 ] = te[ 13 ]; te[ 13 ] = tmp;
    tmp = te[11 ]; te[11 ] = te[ 14 ]; te[ 14 ] = tmp;
}
void mat4_setPosition(float* te, const float* v3) {
    te += 12;
    memcpy(te, v3, sizeof(float) * 3);
}
void mat4_getInverse(float* te, const float* me) {
    float n11 = me[ 0 ], n21 = me[ 1 ], n31 = me[ 2 ], n41 = me[ 3 ];
    float n12 = me[ 4 ], n22 = me[ 5 ], n32 = me[ 6 ], n42 = me[ 7 ];
    float n13 = me[ 8 ], n23 = me[ 9 ], n33 = me[10 ], n43 = me[11 ];
    float n14 = me[12 ], n24 = me[13 ], n34 = me[14 ], n44 = me[15 ];
    
    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;
    
    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    
    if ( det == 0 ) {
        LOG("THREE.Matrix4.getInverse(): can't invert matrix, determinant is 0");
        mat4_identity(te);
        return;
    }
    
    float detInv = 1 / det;
    
    te[ 0 ] = t11 * detInv;
    te[ 1 ] = ( n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44 ) * detInv;
    te[ 2 ] = ( n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44 ) * detInv;
    te[ 3 ] = ( n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43 ) * detInv;
    
    te[ 4 ] = t12 * detInv;
    te[ 5 ] = ( n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44 ) * detInv;
    te[ 6 ] = ( n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44 ) * detInv;
    te[ 7 ] = ( n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43 ) * detInv;
    
    te[ 8 ] = t13 * detInv;
    te[ 9 ] = ( n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44 ) * detInv;
    te[10 ] = ( n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44 ) * detInv;
    te[11 ] = ( n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43 ) * detInv;
    
    te[12 ] = t14 * detInv;
    te[13 ] = ( n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34 ) * detInv;
    te[14 ] = ( n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34 ) * detInv;
    te[15 ] = ( n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33 ) * detInv;
}
void mat4_scale(float* te, const float* v3) {
    float x = v3[0], y = v3[1], z = v3[2];
    te[ 0 ] *= x; te[ 4 ] *= y; te[ 8 ] *= z;
    te[ 1 ] *= x; te[ 5 ] *= y; te[ 9 ] *= z;
    te[ 2 ] *= x; te[ 6 ] *= y; te[10 ] *= z;
    te[ 3 ] *= x; te[ 7 ] *= y; te[11 ] *= z;
}
float mat4_getMaxScaleOnAxis(const float* te) {
    float scaleXSq = te[ 0 ] * te[ 0 ] + te[ 1 ] * te[ 1 ] + te[ 2 ] * te[ 2 ];
    float scaleYSq = te[ 4 ] * te[ 4 ] + te[ 5 ] * te[ 5 ] + te[ 6 ] * te[ 6 ];
    float scaleZSq = te[ 8 ] * te[ 8 ] + te[ 9 ] * te[ 9 ] + te[10 ] * te[10 ];
    return sqrtf(fmaxf(scaleXSq, scaleYSq, scaleZSq));
}
void mat4_makeTranslation(float* m, float x, float y, float z) {
    mat4_set(m,
        1, 0, 0, x,
        0, 1, 0, y,
        0, 0, 1, z,
        0, 0, 0, 1);
}
void mat4_makeRotationX(float* m, float t) {
    float c = cosf(t), s = sinf(t);
    mat4_set((float*)m,
             1, 0,  0, 0,
             0, c, -s, 0,
             0, s,  c, 0,
             0, 0,  0, 1);
}
void mat4_makeRotationY(float* m, float t) {
    float c = cosf(t), s = sinf(t);
    mat4_set((float*)m,
             c, 0, s, 0,
             0, 1, 0, 0,
            -s, 0, c, 0,
             0, 0, 0, 1);
}
void mat4_makeRotationZ(float* m, float t) {
    float c = cosf(t), s = sinf(t);
    mat4_set((float*)m,
             c, -s, 0, 0,
             s,  c, 0, 0,
             0,  0, 1, 0,
             0,  0, 0, 1);
}
void mat4_makeRotationAxis(float* m, const float* axis, float angle) {
    float c  = cosf(angle);
    float s  = sinf(angle);
    float t  = 1 - c;
    float x  = axis[0], y = axis[1], z = axis[2];
    float tx = t * x, ty = t * y;
    
    mat4_set(m,
             tx * x + c, tx * y - s * z, tx * z + s * y, 0,
             tx * y + s * z, ty * y + c, ty * z - s * x, 0,
             tx * z - s * y, ty * z + s * x, t * z * z + c, 0,
             0, 0, 0, 1);
}
void mat4_makeScale(float* m, float x, float y, float z) {
    mat4_set(m,
             x, 0, 0, 0,
             0, y, 0, 0,
             0, 0, z, 0,
             0, 0, 0, 1);
}
void mat4_makeShear(float* m, float x, float y, float z) {
    mat4_set(m,
             1, y, z, 0,
             x, 1, z, 0,
             x, y, 1, 0,
             0, 0, 0, 1);
}
void mat4_compose(float* m4, const float* p, const float* q, const float* s) {
    mat4_makeRotationFromQuaternion(m4, q);
    mat4_scale(m4, s);
    mat4_setPosition(m4, p);
}
void mat4_makePerspective(float* te, float left, float right, float top, float bottom, float near, float far) {
    float x = 2 * near / ( right - left );
    float y = 2 * near / ( top - bottom );
    
    float a = ( right + left ) / ( right - left );
    float b = ( top + bottom ) / ( top - bottom );
    float c = - ( far + near ) / ( far - near );
    float d = - 2 * far * near / ( far - near );
    
    te[ 0 ] = x;	te[ 4 ] = 0;	te[ 8 ] = a;	te[ 12 ] = 0;
    te[ 1 ] = 0;	te[ 5 ] = y;	te[ 9 ] = b;	te[ 13 ] = 0;
    te[ 2 ] = 0;	te[ 6 ] = 0;	te[10 ] = c;	te[ 14 ] = d;
    te[ 3 ] = 0;	te[ 7 ] = 0;	te[11 ] = -1;	te[ 15 ] = 0;
}
void mat4_makeOrthographic(float* te, float left, float right, float top, float bottom, float near, float far) {
    float w = 1.0 / ( right - left );
    float h = 1.0 / ( top - bottom );
    float p = 1.0 / ( far - near );
    
    float x = ( right + left ) * w;
    float y = ( top + bottom ) * h;
    float z = ( far + near ) * p;
    
    te[ 0 ] = 2 * w;	te[ 4 ] = 0;        te[ 8 ] = 0;        te[ 12 ] = -x;
    te[ 1 ] = 0;        te[ 5 ] = 2 * h;	te[ 9 ] = 0;        te[ 13 ] = -y;
    te[ 2 ] = 0;        te[ 6 ] = 0;        te[10 ] = -2 * p;   te[ 14 ] = -z;
    te[ 3 ] = 0;        te[ 7 ] = 0;        te[11 ] = 0;        te[ 15 ] = 1;
}
bool mat4_equals(const float* a, const float* b) {
    return 0 == memcmp(a, b, sizeof(float) * 16);
}
void mat4_fromArray(float* dst, const float* array, int offset) {
    array += offset;
    memcpy(dst, array, sizeof(float) * 16);
}
void mat4_toArray(const float* dst, float* array, int offset) {
    array += offset;
    memcpy(array, dst, sizeof(float) * 16);
}
