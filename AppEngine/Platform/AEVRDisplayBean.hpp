//
//  AEVRDisplayBean.hpp
//  AppEngine
//
//  Created by 韩琼 on 2016/12/3.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEVRDisplayBean_hpp
#define AEVRDisplayBean_hpp

#include "AEPlatform.hpp"

class AEManager;
class AEVRDisplay;
class AEVRDisplayBean {
private:
    jobject      impl;
    AEManager*   manager;
    AEVRDisplay* display;
    
public:
    ~AEVRDisplayBean();
    AEVRDisplayBean(AEVRDisplay* display);
    
    void init(AEManager* mgr);
    void stop();
    void start();
    void update();
};

#endif /* AEVRDisplayBean_hpp */
