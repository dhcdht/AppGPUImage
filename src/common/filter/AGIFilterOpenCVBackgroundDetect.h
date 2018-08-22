//
//  AGIFilterOpenCVBackgroundDetect.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/15.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterOpenCVBackgroundDetect_h
#define AGIFilterOpenCVBackgroundDetect_h

#include "AGIFilter.h"
#include "opencv/cv.hpp"


class AGIFilterOpenCVBackgroundDetect : public AGIFilter
{
public:
	AGIFilterOpenCVBackgroundDetect();
	~AGIFilterOpenCVBackgroundDetect() override;

public:
	bool prepareFilter() override;
	bool submit() override;

private:
	cv::Ptr<cv::BackgroundSubtractor> m_bgDetector;
};


#endif /* AGIFilterOpenCVBackgroundDetect_h */
