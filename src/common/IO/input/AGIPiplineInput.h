//
//  AGIPiplineInput.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineInput_h
#define AGIPiplineInput_h

#include "piplineIO/AGIPiplineIO.hpp"
#include "IO/AGIImage.h"
#include "util/bgfxUtils.h"


class AGIPiplineInput : public AGIPiplineSource<AGIImagePtr>
{
public:
	AGIPiplineInput();
	virtual ~AGIPiplineInput();

	virtual bool init(const std::string filePath);

public:
	virtual Milliseconds getDuration();
	virtual int getPreferFrameRate();
	virtual bool syncSeekToTime(Milliseconds time);
	virtual Milliseconds getCurrentTime();
};
typedef std::shared_ptr<AGIPiplineInput> AGIPiplineInputPtr;


#endif /* AGIPiplineInput_h */
