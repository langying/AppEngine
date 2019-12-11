//
//  AEVRDisplayImpl.hpp
//  AppEngine
//
//  Created by 韩琼 on 16/8/23.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#ifndef AEVRDisplayImpl_hpp
#define AEVRDisplayImpl_hpp

#include "AEPlatform.hpp"

#include "EyeParams.h"
#include "HeadTracker.h"
#include "HeadTransform.h"
#include "HeadMountedDisplay.h"

class AEVRDisplayImpl {
public:
    ~AEVRDisplayImpl();
    AEVRDisplayImpl();
    void stopTracking();
    void startTracking();
    void update(std::function<void(GLKQuaternion* q, FieldOfView* left, FieldOfView* right)> handle);
    
private:
    float zNear, zFar;
    float distortionCorrectionScale;
    bool  distortionCorrectionEnabled;
    
private:
    EyeParams* leftEyeParams;
    EyeParams* rightEyeParams;
    HeadTracker* headTracker;
    HeadTransform* headTransform;
    HeadMountedDisplay* headMountedDisplay;
};

#endif /* AEVRDisplayImpl_hpp */
