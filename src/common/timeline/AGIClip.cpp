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
	: m_input{}
	, m_output{}
	, m_filterGraph{}
	, m_readerBeginTime{ 0 }
	, m_readerEndTime{ -1 }
	, m_trackBeginTime{ 0 }
	, m_trackEndTime{ -1 }
{

}

AGIClip::~AGIClip()
{
	m_input = nullptr;
	m_output = nullptr;
	m_filterGraph = nullptr;

	m_readerBeginTime = Milliseconds(0);
	m_readerEndTime = Milliseconds(-1);
	m_trackBeginTime = Milliseconds(0);
	m_trackEndTime = Milliseconds(-1);
}

bool AGIClip::init(const std::string filePath)
{
	m_input = std::make_shared<AGIPiplineInputFFmpegReader>();
	if (!m_input->init(filePath))
	{
		return false;
	}

	m_output = std::make_shared<AGIPiplineIOImage>();
	if (!m_output->init())
	{
		return false;
	}
	m_output->setInputParamsDelegate(m_input);

	m_input->addTarget(m_output);

	m_filterGraph = std::make_shared<AGIFilterGraph>();
	m_filterGraph->addSource(m_input);
	m_filterGraph->addTarget(m_output);

	m_readerBeginTime = Milliseconds(0);
	m_readerEndTime = Milliseconds(-1);
	m_trackBeginTime = Milliseconds(0);
	m_trackEndTime = Milliseconds(-1);

	return true;
}

Milliseconds AGIClip::getReaderBeginTime()
{
	return m_readerBeginTime;
}

bool AGIClip::setReaderBeginTime(Milliseconds readerBeginTime)
{
	m_readerBeginTime = readerBeginTime;

	return true;
}

Milliseconds AGIClip::getReaderEndTime()
{
	if (m_readerEndTime == Milliseconds(-1))
	{
		if (m_input == nullptr)
		{
			return Milliseconds(0);
		}

		return m_input->getDuration();
	}
	else
	{
		return m_readerEndTime;
	}
}

bool AGIClip::setReaderEndTime(std::chrono::milliseconds readerEndTime)
{
	m_readerEndTime = readerEndTime;

	return true;
}

Milliseconds AGIClip::getTrackBeginTime()
{
	return m_trackBeginTime;
}

bool AGIClip::setTrackBeginTime(std::chrono::milliseconds trackBeginTime)
{
	m_trackBeginTime = trackBeginTime;

	return true;
}

Milliseconds AGIClip::getTrackEndTime()
{
	if (m_trackEndTime == Milliseconds(-1))
	{
		return this->getReaderEndTime();
	}

	return m_trackEndTime;
}

bool AGIClip::setTrackEndTime(std::chrono::milliseconds trackEndTime)
{
	m_trackEndTime = trackEndTime;

	return true;
}

AGIFilterGraphPtr AGIClip::getFilterGraph()
{
	return m_filterGraph;
}

AGIPiplineIOImagePtr AGIClip::getFilterGraphOutput()
{
	return m_output;
}

//region AGIPiplineInput

Milliseconds AGIClip::getDuration()
{
	auto trackEndTime = this->getTrackEndTime();
	auto trackBeginTime = this->getTrackBeginTime();

	return (trackEndTime - trackBeginTime);
}

int AGIClip::getPreferFrameRate()
{
	return m_input->getPreferFrameRate();
}

bool AGIClip::syncSeekToTime(Milliseconds time)
{
	return m_input->syncSeekToTime(time);
}

Milliseconds AGIClip::getCurrentFrameTime()
{
	return m_input->getCurrentFrameTime();
}

Milliseconds AGIClip::getCurrentFrameDuration()
{
	return m_input->getCurrentFrameDuration();
}

//region AGIPiplineSource

std::vector<AGIImagePtr> AGIClip::pullOutputs()
{
	if (!m_output->processTarget())
	{
		return {};
	}

	auto outputImage = m_output->getLastOutputImage();

	return { outputImage };
}

void AGIClip::endOneProcess()
{
	m_output->endOneProcess();
}

//endregion AGIPiplineSource

//endregion AGIPiplineInput

