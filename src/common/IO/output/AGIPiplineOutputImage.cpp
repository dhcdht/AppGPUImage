//
//  AGIPiplineOutputImage.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineOutputImage.h"


AGIPiplineOutputImage::AGIPiplineOutputImage()
	: m_lastOutputImage{nullptr}
{
	
}

AGIPiplineOutputImage::~AGIPiplineOutputImage()
{
	m_lastOutputImage = nullptr;
}

bool AGIPiplineOutputImage::init()
{
	return true;
}

bool AGIPiplineOutputImage::processTarget()
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

void AGIPiplineOutputImage::endOneProcess()
{
	m_lastOutputImage = nullptr;

	AGIPiplineOutput::endOneProcess();
}

AGIImagePtr AGIPiplineOutputImage::getLastOutputImage()
{
	return m_lastOutputImage;
}

