//
//  AGIClip.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIClip_h
#define AGIClip_h

#include <iostream>
#include <chrono>
#include "IO/input/AGIPiplineInput.h"
#include "util/bgfxUtils.h"


class AGIClip
{
public:
	AGIClip();
	virtual ~AGIClip();

	bool init(const std::string filePath);

public:
	Milliseconds getReaderBeginTime();
	Milliseconds getReaderEndTime();
	Milliseconds getTrackBeginTime();
	Milliseconds getTrackEndTime();

private:
	AGIPiplineInputPtr m_piplineInput;

	Milliseconds m_readerBeginTime;
	Milliseconds m_readerEndTime;
	Milliseconds m_trackBeginTime;
	Milliseconds m_trackEndTime;
};
typedef std::shared_ptr<AGIClip> AGIClipPtr;


#endif /* AGIClip_h */