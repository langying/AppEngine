//
//  AEEuler.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/11.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEEuler_hpp
#define AEEuler_hpp

#include "AEPlatform.hpp"

void eule_setFromRotationMatrix(float* dst, const float* m, const char* order);
void eule_setFromQuaternion(float* dst, const float* quaternion, const char* order);
void eule_reorder(float* dst, const char* order, const char* newer);

#endif /* AEEuler_hpp */
