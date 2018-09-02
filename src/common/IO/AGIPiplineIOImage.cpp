//
//  AGIPiplineIOImage.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineIOImage.h"


AGIPiplineIOImage::AGIPiplineIOImage()
	: m_lastOutputImage{nullptr}
	, m_inputParamsDelegate{nullptr}
{
	
}

AGIPiplineIOImage::~AGIPiplineIOImage()
{
	m_lastOutputImage = nullptr;
	m_inputParamsDelegate = nullptr;
}

bool AGIPiplineIOImage::init()
{
	return true;
}

void AGIPiplineIOImage::setInputParamsDelegate(AGIPiplineInputPtr inputParamsDelegate)
{
	m_inputParamsDelegate = inputParamsDelegate;
}

AGIPiplineInputPtr AGIPiplineIOImage::getInputParamsDelegate()
{
	return m_inputParamsDelegate;
}

std::vector<AGIImagePtr> AGIPiplineIOImage::pullOutputs()
{
	if (m_lastOutputImage != nullptr)
	{
		return { m_lastOutputImage };
	}

	if (!this->processTarget())
	{
		return {};
	}

	if (m_lastOutputImage != nullptr)
	{
		return { m_lastOutputImage };
	}
	else
	{
		return {};
	}
}

Milliseconds AGIPiplineIOImage::getDuration()
{
	return m_inputParamsDelegate->getDuration();
}

int AGIPiplineIOImage::getPreferFrameRate()
{
	return m_inputParamsDelegate->getPreferFrameRate();
}

bool AGIPiplineIOImage::syncSeekToTime(Milliseconds time)
{
	return m_inputParamsDelegate->syncSeekToTime(time);
}

Milliseconds AGIPiplineIOImage::getCurrentFrameTime()
{
	return m_inputParamsDelegate->getCurrentFrameTime();
}

Milliseconds AGIPiplineIOImage::getCurrentFrameDuration()
{
	return m_inputParamsDelegate->getCurrentFrameDuration();
}

bool AGIPiplineIOImage::processTarget()
{
	if (m_lastOutputImage != nullptr)
	{
		return true;
	}

	AGIImagePtr lastOutput;
	for (auto weakSource : m_sources) {
		auto source = weakSource.lock();
		if (source == nullptr) {
			continue;
		}
		auto sourceOutputs = source->pullOutputs();
		if (sourceOutputs.empty()) {
			continue;
		}
		lastOutput = sourceOutputs.front();
	}
	if (lastOutput == nullptr || !bgfx::isValid(lastOutput->getGPUTextureHandle())) {
		return false;
	}

	m_lastOutputImage = lastOutput;
	return true;
}

void AGIPiplineIOImage::endOneProcess()
{
	m_lastOutputImage = nullptr;

	AGIPiplineOutput::endOneProcess();
}

AGIImagePtr AGIPiplineIOImage::getLastOutputImage()
{
	return m_lastOutputImage;
}

