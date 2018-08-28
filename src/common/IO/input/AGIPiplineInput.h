//
//  AGIPiplineInput.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineInput_h
#define AGIPiplineInput_h

#include "pipline/AGIPipline.hpp"
#include "IO/AGIImage.h"
#include "util/bgfxUtils.h"


class AGIPiplineInput : public AGIPiplineSource<AGIImagePtr>
{
public:
	AGIPiplineInput();
	virtual ~AGIPiplineInput();

	virtual bool init(const std::string filePath) = 0;

public:
	virtual Milliseconds getDuration() = 0;
	virtual int getPreferFrameRate() = 0;
	virtual bool syncSeekToTime(Milliseconds time) = 0;
	virtual Milliseconds getCurrentFrameTime() = 0;
	virtual Milliseconds getCurrentFrameDuration() = 0;
};
typedef std::shared_ptr<AGIPiplineInput> AGIPiplineInputPtr;


#endif /* AGIPiplineInput_h */
