//
//  AGIClip.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIClip.h"
#include "IO/input/AGIPiplineInputFFmpegReader.h"


AGIClip::AGIClip()
	: m_piplineInput{ nullptr }
	, m_readerBeginTime{ 0 }
	, m_readerEndTime{ -1 }
	, m_trackBeginTime{ 0 }
	, m_trackEndTime{ -1 }
{

}

AGIClip::~AGIClip()
{
	m_piplineInput = nullptr;

	m_readerBeginTime = Milliseconds(0);
	m_readerEndTime = Milliseconds(-1);
	m_trackBeginTime = Milliseconds(0);
	m_trackEndTime = Milliseconds(-1);
}

bool AGIClip::init(const std::string filePath)
{
	m_piplineInput = std::make_shared<AGIPiplineInputFFmpegReader>();
	if (!m_piplineInput->init(filePath))
	{
		return false;
	}

	m_readerEndTime = m_piplineInput->getDuration();

	return true;
}

Milliseconds AGIClip::getReaderBeginTime()
{
	return m_readerBeginTime;
}

Milliseconds AGIClip::getReaderEndTime()
{
	if (m_readerEndTime == Milliseconds(-1))
	{
		if (m_piplineInput == nullptr)
		{
			return Milliseconds(0);
		}

		return m_piplineInput->getDuration();
	}
	else
	{
		return m_readerEndTime;
	}
}

Milliseconds AGIClip::getTrackBeginTime()
{
	return m_trackBeginTime;
}

Milliseconds AGIClip::getTrackEndTime()
{
	if (m_trackEndTime == Milliseconds(-1))
	{
		return this->getReaderEndTime();
	}

	return m_trackEndTime;
}

