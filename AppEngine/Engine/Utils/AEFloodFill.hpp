//
//  AEFloodFill.hpp
//  AppEngine
//
//  Created by 韩琼 on 2017/2/22.
//  Copyright © 2017年 AppEngine. All rights reserved.
//

#ifndef AEFloodFill_hpp
#define AEFloodFill_hpp

#include "AEPlatform.hpp"

void floodfill(AEColor pen, AEColor* img, int x, int y, int w, int h, uint8_t tolerance);

#endif /* AEFloodFill_hpp */
