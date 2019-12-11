//
//  AEEuler.cpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/11.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#include "AEEuler.hpp"
#include "AEMatrix4.hpp"

void eule_setFromRotationMatrix(float* e, const float* te, const char* _order) {
    // assumes the upper 3x3 of m is a pure rotation matrix (i.e, unscaled)
    
    float m11 = te[ 0 ], m12 = te[ 4 ], m13 = te[ 8 ];
    float m21 = te[ 1 ], m22 = te[ 5 ], m23 = te[ 9 ];
    float m31 = te[ 2 ], m32 = te[ 6 ], m33 = te[10 ];
    std::string order = _order ? _order : "";

    if ( order == "XYZ" ) {
        
        e[1] = asinf( clampf( m13, - 1, 1 ) );
        
        if ( fabsf( m13 ) < 0.99999 ) {
            
            e[0] = atan2f( - m23, m33 );
            e[2] = atan2f( - m12, m11 );
            
        } else {
            
            e[0] = atan2f( m32, m22 );
            e[2] = 0;
            
        }
        
    } else if ( order == "YXZ" ) {
        
        e[0] = asinf( - clampf( m23, - 1, 1 ) );
        
        if ( fabsf( m23 ) < 0.99999 ) {
            
            e[1] = atan2f( m13, m33 );
            e[2] = atan2f( m21, m22 );
            
        } else {
            
            e[1] = atan2f( - m31, m11 );
            e[2] = 0;
            
        }
        
    } else if ( order == "ZXY" ) {
        
        e[0] = asinf( clampf( m32, - 1, 1 ) );
        
        if ( fabsf( m32 ) < 0.99999 ) {
            
            e[1] = atan2f( - m31, m33 );
            e[2] = atan2f( - m12, m22 );
            
        } else {
            
            e[1] = 0;
            e[2] = atan2f( m21, m11 );
            
        }
        
    } else if ( order == "ZYX" ) {
        
        e[1] = asinf( - clampf( m31, - 1, 1 ) );
        
        if ( fabsf( m31 ) < 0.99999 ) {
            
            e[0] = atan2f( m32, m33 );
            e[2] = atan2f( m21, m11 );
            
        } else {
            
            e[0] = 0;
            e[2] = atan2f( - m12, m22 );
            
        }
        
    } else if ( order == "YZX" ) {
        
        e[2] = asinf( clampf( m21, - 1, 1 ) );
        
        if ( fabsf( m21 ) < 0.99999 ) {
            
            e[0] = atan2f( - m23, m22 );
            e[1] = atan2f( - m31, m11 );
            
        } else {
            
            e[0] = 0;
            e[1] = atan2f( m13, m33 );
            
        }
        
    } else if ( order == "XZY" ) {
        
        e[2] = asinf( - clampf( m12, - 1, 1 ) );
        
        if ( fabsf( m12 ) < 0.99999 ) {
            
            e[0] = atan2f( m32, m22 );
            e[1] = atan2f( m13, m11 );
            
        } else {
            
            e[0] = atan2f( - m23, m33 );
            e[1] = 0;
            
        }
        
    } else {
        
        LOG( "THREE.Euler: .setFromRotationMatrix() given unsupported order: %s", order.c_str() );
        
    }
}

void eule_setFromQuaternion(float* dst, const float* q, const char* order) {
    static float matrix[16];
    mat4_makeRotationFromQuaternion(matrix, q);
    eule_setFromRotationMatrix(dst, matrix, order);
}
void eule_reorder(float* dst, const char* order, const char* newer) {
    static float q[16];
    quat_setFromEuler(q, dst, order);
    eule_setFromQuaternion(dst, q, newer);
}
