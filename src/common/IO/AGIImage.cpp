//
//  AGIImage.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/30.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIImage.h"
#include "../common_deps/bgfx/src/renderer.h"


AGIImage::AGIImage(bgfx::TextureHandle textureHandle, int sizeWidth, int sizeHeight, bgfx::TextureFormat::Enum format)
	: m_CPUBuffer(nullptr)
	, m_GPUTextureHandle(textureHandle)
	, m_GPUFramebufferHandle(BGFX_INVALID_HANDLE)
	, m_sizeWidth(sizeWidth)
	, m_sizeHeight(sizeHeight)
	, m_format(format)
{

}

AGIImage::AGIImage(bgfx::FrameBufferHandle framebufferHandle, int sizeWidth, int sizeHeight, bgfx::TextureFormat::Enum format)
	: m_CPUBuffer(nullptr)
	, m_GPUTextureHandle(BGFX_INVALID_HANDLE)
	, m_GPUFramebufferHandle(framebufferHandle)
	, m_sizeWidth(sizeWidth)
	, m_sizeHeight(sizeHeight)
	, m_format(format)
{
	
}

AGIImage::~AGIImage()
{
	if (m_CPUBuffer != nullptr)
	{
		bgfx::release(m_CPUBuffer);
		m_CPUBuffer = nullptr;
	}
	if (bgfx::isValid(m_GPUTextureHandle))
	{
		bgfx::destroy(m_GPUTextureHandle);
		m_GPUTextureHandle = BGFX_INVALID_HANDLE;
	}
	if (bgfx::isValid(m_GPUFramebufferHandle))
	{
		bgfx::destroy(m_GPUFramebufferHandle);
		m_GPUFramebufferHandle = BGFX_INVALID_HANDLE;
	}
	m_sizeWidth = 0;
	m_sizeHeight = 0;
	m_format = bgfx::TextureFormat::Enum::Count;
}

bgfx::Memory* AGIImage::getCPUBuffer()
{
	return m_CPUBuffer;
}

bgfx::Memory* AGIImage::getCPUBufferFocre()
{
    if (m_CPUBuffer != nullptr) {
        return m_CPUBuffer;
    }
    auto textureHandle = getGPUTextureHandle();
    if (!bgfx::isValid(textureHandle)) {
        return nullptr;
    }

    // TODO: 是否可以检查 textureHandle 能否被 read back，这样可以少用一个 texture
    auto readbackTexture = bgfx::createTexture2D(getSizeWidth(), getSizeHeight(), false, 1, getFormat(), BGFX_TEXTURE_READ_BACK);
    bgfx::blit(0, readbackTexture, 0, 0, textureHandle);
    m_CPUBuffer = const_cast<bgfx::Memory*>(bgfx::alloc(4 * getSizeWidth() * getSizeHeight()));
    auto completeFrameID = bgfx::readTexture(readbackTexture, m_CPUBuffer->data);
    // https://github.com/bkaradzic/bgfx/issues/802
    // 根据上边链接的讨论 frame 2 次可以保证 readTexture 操作被执行完
    auto frameID = bgfx::frame();
    frameID = bgfx::frame();
	bgfx::destroy(readbackTexture);
    if (frameID < completeFrameID) {
        bgfx::release(m_CPUBuffer);
        m_CPUBuffer = nullptr;

        return nullptr;
    }

    return m_CPUBuffer;
}

bgfx::TextureHandle AGIImage::getGPUTextureHandle()
{
	if (bgfx::isValid(m_GPUTextureHandle))
	{
		return m_GPUTextureHandle;
	}

	if (bgfx::isValid(m_GPUFramebufferHandle))
	{
		auto textureHandle = bgfx::getTexture(m_GPUFramebufferHandle);
		m_GPUTextureHandle = textureHandle;

		return m_GPUTextureHandle;
	}

	return BGFX_INVALID_HANDLE;
}

int AGIImage::getSizeWidth()
{
	return m_sizeWidth;
}

int AGIImage::getSizeHeight()
{
	return m_sizeHeight;
}

bgfx::TextureFormat::Enum AGIImage::getFormat()
{
	return m_format;
}

