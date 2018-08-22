//
//  AGIFilterOpenCVBackgroundDetect.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/15.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterOpenCVBackgroundDetect.h"
#include "util/bgfxUtils.h"
#include "util/AGIUtilDefer.h"
#include "../common_deps/bgfx/src/renderer.h"


static auto kBGDetector = cv::createBackgroundSubtractorKNN();


AGIFilterOpenCVBackgroundDetect::AGIFilterOpenCVBackgroundDetect()
	: m_bgDetector{}
{
	
}

AGIFilterOpenCVBackgroundDetect::~AGIFilterOpenCVBackgroundDetect()
{
	m_bgDetector.release();
}

bool AGIFilterOpenCVBackgroundDetect::prepareFilter()
{
	if (m_isPrepared)
	{
		return true;
	}

	m_bgDetector = cv::createBackgroundSubtractorKNN();
	if (m_bgDetector == nullptr)
	{
		return false;
	}

	return AGIFilter::prepareFilter();
}

bool AGIFilterOpenCVBackgroundDetect::submit()
{
	if (m_textures.size() < 1)
	{
		return false;
	}

	// 让 gpu 将 m_textures[0] 绘制出来
	bgfx::frame();
	auto videoFrame = m_textures[0];
	auto width = videoFrame->getSizeWidth();
	auto height = videoFrame->getSizeHeight();
	auto cpuBuffer = videoFrame->getCPUBufferFocre();
	cv::Mat frame = bgfxUtils::generateCVMatFromBuffer(cpuBuffer, bgfx::TextureFormat::Enum::RGBA8, width, height);
	cv::Mat bgMask;
	kBGDetector->apply(frame, bgMask);
	auto maskBuffer = const_cast<bgfx::Memory*>(bgfx::alloc(bgMask.rows * bgMask.cols));
	memcpy(maskBuffer->data, bgMask.data, maskBuffer->size);
	auto maskTextureHandle = bgfxUtils::generateTextureFromBuffer(maskBuffer, bgfx::TextureFormat::Enum::R8, width, height);
	bgfx::release(maskBuffer);

	m_framebufferImage = std::make_shared<AGIImage>(maskTextureHandle, width, height, bgfx::TextureFormat::Enum::R8);

	if (!commitVertex()) {
		return false;
	}
	if (!m_program->setTexture("s_texture0", maskTextureHandle)) {
		return false;
	}
	if (!commitState()) {
		return false;
	}
	if (!submitProgram()) {
		return false;
	}

	return true;
}

