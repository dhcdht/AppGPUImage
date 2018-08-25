//
//  AGIFilterOpenCVFaceDetect.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/20.
//  Copyright © 2018年 董宏昌. All rights reserved.
//
#ifndef AGIFilterOpenCVFaceDetect_h
#define AGIFilterOpenCVFaceDetect_h

#include "AGIFilter.h"
#include "opencv/cv.hpp"


class AGIFilterOpenCVFaceDetect : public AGIFilter
{
public:
    AGIFilterOpenCVFaceDetect();
    ~AGIFilterOpenCVFaceDetect() override;

public:
    bool prepareFilter() override;
    bool submit() override;

private:
    cv::Ptr<cv::CascadeClassifier> m_classifier;
};


#endif /* AGIFilterOpenCVFaceDetect_h */