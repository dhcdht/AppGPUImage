//
//  AGIContext.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIContext.h"
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/debug.h>

extern "C" {
#include "libavutil/log.h"
}


AGIContext::AGIContext()
: m_platformData({nullptr})
, m_defaultAllocator(new bx::DefaultAllocator())
, m_curValidViewID(0)
, m_videoProcessQueue(new AGIDispatchQueue("com.dhcdht.AppGPUImage.videoProcessQueue"))
{
}

AGIContext::~AGIContext()
{
	if (m_defaultAllocator != nullptr)
	{
		delete m_defaultAllocator;
		m_defaultAllocator = nullptr;
	}
	if (m_videoProcessQueue != nullptr)
	{
		delete m_videoProcessQueue;
		m_videoProcessQueue = nullptr;
	}
}

AGIContext* AGIContext::sharedContext()
{
    static auto kSharedContext = new AGIContext();

    return kSharedContext;
}

bool AGIContext::init(void *window, int windowSizeWidth, int windowSizeHeight)
{
    m_window = window;
    m_windowSizeWidth = windowSizeWidth;
    m_windowSizeHeight = windowSizeHeight;

    m_platformData.nwh = m_window;
    bgfx::setPlatformData(m_platformData);

    // 在 bgfx::init 之前调用 renderFrame 可以让 bgfx 进入单线程模式
    // https://bkaradzic.github.io/bgfx/internals.html
    bgfx::renderFrame();

    bgfx::Init init;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.resolution.width = m_windowSizeWidth;
    init.resolution.height = m_windowSizeHeight;
    bgfx::init(init);
    
    // 目前 ffmpeg 会有 No accelerated colorspace conversion found from yuv420p to rgba 的 warning
//    av_log_set_level(AV_LOG_WARNING);
    av_log_set_level(AV_LOG_ERROR);

    bgfx::setViewRect(0, 0, 0, m_windowSizeWidth, m_windowSizeHeight);
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
    bgfx::touch(0);
    bgfx::frame();

    return true;
}

bx::AllocatorI* AGIContext::getDefaultAllocator()
{
    return m_defaultAllocator;
}

void AGIContext::setWindowSizeChange(int windowSizeWidth, int windowSizeHeight)
{
    m_windowSizeWidth = windowSizeWidth;
    m_windowSizeHeight = windowSizeHeight;
}

int AGIContext::getWindowSizeWidth()
{
    return m_windowSizeWidth;
}

int AGIContext::getWindowSizeHeight()
{
    return m_windowSizeHeight;
}

bgfx::ViewId AGIContext::getCurValidViewID()
{
    return m_curValidViewID;
}

bgfx::ViewId AGIContext::nextValidViewID()
{
    m_curValidViewID++;

    return m_curValidViewID;
}

bgfx::ViewId AGIContext::resetValidViewID()
{
    m_curValidViewID = 0;

    return m_curValidViewID;
}

AGIDispatchQueue* AGIContext::getVideoProcessQueue()
{
	return m_videoProcessQueue;
}


