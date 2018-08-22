//
//  AGIPiplineInput.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineInput.h"


AGIPiplineInput::AGIPiplineInput()
{
	
}

AGIPiplineInput::~AGIPiplineInput()
{
	
}

bool AGIPiplineInput::init(const std::string filePath)
{
	return true;
}

Milliseconds AGIPiplineInput::getDuration()
{
	return Milliseconds(0);
}

int AGIPiplineInput::getPreferFrameRate()
{
	return 30;
}


bool AGIPiplineInput::syncSeekToTime(Milliseconds time)
{
	return true;
}

Milliseconds AGIPiplineInput::getCurrentTime()
{
	return Milliseconds(0);
}

