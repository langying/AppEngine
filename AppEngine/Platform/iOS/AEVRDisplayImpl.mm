//
//  AEVRDisplayImpl.mm
//  AppEngine
//
//  Created by 韩琼 on 2016/8/23.
//  Copyright © 2016年 AppEngine. All rights reserved.
//

#include "AEVRDisplayImpl.hpp"

AEVRDisplayImpl::~AEVRDisplayImpl() {
    delete leftEyeParams;
    delete rightEyeParams;
    delete headTracker;
    delete headTransform;
    delete headMountedDisplay;
}
AEVRDisplayImpl::AEVRDisplayImpl() {
    zNear = 0.1f;
    zFar  = 100.0f;
    distortionCorrectionScale = 1.0f;
    distortionCorrectionEnabled = YES;
    
    leftEyeParams      = new EyeParams(EyeParamsTypeLeft);
    rightEyeParams     = new EyeParams(EyeParamsTypeRight);
    headTracker        = new HeadTracker();
    headTransform      = new HeadTransform();
    headMountedDisplay = new HeadMountedDisplay([UIScreen mainScreen]);
}
void AEVRDisplayImpl::stopTracking() {
    headTracker->stopTracking();
}
void AEVRDisplayImpl::startTracking() {
    headTracker->startTracking();
}
void AEVRDisplayImpl::update(std::function<void(GLKQuaternion* q, FieldOfView* left, FieldOfView* right)> handle) {
    ScreenParams *screenParams = headMountedDisplay->getScreen();
    CardboardDeviceParams *cardboardDeviceParams = headMountedDisplay->getCardboard();
    headTransform->setHeadView(headTracker->getLastHeadView());
    
    float halfInterpupillaryDistance = cardboardDeviceParams->getInterpupillaryDistance() * 0.5f;
    
    GLKMatrix4 leftEyeTranslate  = GLKMatrix4Identity;
    GLKMatrix4 rightEyeTranslate = GLKMatrix4Identity;
    GLKMatrix4Translate(leftEyeTranslate,  +halfInterpupillaryDistance, 0, 0);
    GLKMatrix4Translate(rightEyeTranslate, -halfInterpupillaryDistance, 0, 0);
    
    leftEyeParams->getTransform()->setEyeView(GLKMatrix4Multiply(leftEyeTranslate, headTransform->getHeadView()));
    rightEyeParams->getTransform()->setEyeView(GLKMatrix4Multiply(rightEyeTranslate, headTransform->getHeadView()));
    
    float eyeToScreenDistance = cardboardDeviceParams->getVisibleViewportSize() / 2.0f / tanf(GLKMathDegreesToRadians(cardboardDeviceParams->getFovY()) / 2.0f );
    
    
    float left   = screenParams->getWidthMeters() / 2.0f - halfInterpupillaryDistance;
    float right  = halfInterpupillaryDistance;
    float bottom = cardboardDeviceParams->getVerticalDistanceToLensCenter() - screenParams->getBorderSizeMeters();
    float top    = screenParams->getBorderSizeMeters() + screenParams->getHeightMeters() - cardboardDeviceParams->getVerticalDistanceToLensCenter();
    
    FieldOfView *leftEyeFov = leftEyeParams->getFov();
    leftEyeFov->setLeft(GLKMathRadiansToDegrees(atan2f(left, eyeToScreenDistance)));
    leftEyeFov->setRight(GLKMathRadiansToDegrees(atan2f(right, eyeToScreenDistance)));
    leftEyeFov->setBottom(GLKMathRadiansToDegrees(atan2f(bottom, eyeToScreenDistance)));
    leftEyeFov->setTop(GLKMathRadiansToDegrees(atan2f(top, eyeToScreenDistance)));
    
    FieldOfView *rightEyeFov = rightEyeParams->getFov();
    rightEyeFov->setLeft(leftEyeFov->getRight());
    rightEyeFov->setRight(leftEyeFov->getLeft());
    rightEyeFov->setBottom(leftEyeFov->getBottom());
    rightEyeFov->setTop(leftEyeFov->getTop());
    
    leftEyeParams->getTransform()->setPerspective(leftEyeFov->toPerspectiveMatrix(zNear, zFar));
    rightEyeParams->getTransform()->setPerspective(rightEyeFov->toPerspectiveMatrix(zNear, zFar));
    
    leftEyeParams->getViewport()->setViewport(0, 0, screenParams->getWidth() / 2, screenParams->getHeight());
    rightEyeParams->getViewport()->setViewport(screenParams->getWidth() / 2, 0, screenParams->getWidth() / 2, screenParams->getHeight());
    
    
    int leftX  = leftEyeParams->getViewport()->x;
    int leftY  = leftEyeParams->getViewport()->y;
    int leftW  = leftEyeParams->getViewport()->width;
    int leftH  = leftEyeParams->getViewport()->height;
    int rightX = rightEyeParams->getViewport()->x;
    int rightY = rightEyeParams->getViewport()->y;
    int rightW = rightEyeParams->getViewport()->width;
    int rightH = rightEyeParams->getViewport()->height;
    
    leftEyeParams->getViewport()->setViewport((int)(leftX * distortionCorrectionScale),
                                              (int)(leftY * distortionCorrectionScale),
                                              (int)(leftW * distortionCorrectionScale),
                                              (int)(leftH * distortionCorrectionScale));
    
    rightEyeParams->getViewport()->setViewport((int)(rightX * distortionCorrectionScale),
                                               (int)(rightY * distortionCorrectionScale),
                                               (int)(rightW * distortionCorrectionScale),
                                               (int)(rightH * distortionCorrectionScale));
    
    leftEyeParams->getViewport()->setViewport(leftX, leftY, leftW, leftH);
    rightEyeParams->getViewport()->setViewport(rightX, rightY, rightW, rightH);
    
    GLKQuaternion q = headTransform->getQuaternion();
    handle(&q, leftEyeParams->getFov(), rightEyeParams->getFov());
}
