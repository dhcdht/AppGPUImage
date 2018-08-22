//
//  AGIPiplineOutputContextWindow.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineOutputContextWindow.h"
#include "core/AGIContext.h"


AGIPiplineOutputContextWindow::AGIPiplineOutputContextWindow()
{

}

AGIPiplineOutputContextWindow::~AGIPiplineOutputContextWindow()
{
    m_filter = nullptr;
}

bool AGIPiplineOutputContextWindow::init()
{
    m_filter = std::make_shared<AGIFilter>();
    if (!m_filter->prepareFilter()) {
        return false;
    }
    if (!m_filter->setSubmitViewID(0)) {
        return false;
    }

    return true;
}

bool AGIPiplineOutputContextWindow::processTarget()
{
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

    auto viewID = AGIContext::sharedContext()->nextValidViewID();
    int sizeWidth = AGIContext::sharedContext()->getWindowSizeWidth();
    int sizeHeight = AGIContext::sharedContext()->getWindowSizeHeight();
    bgfx::setViewFrameBuffer(viewID, BGFX_INVALID_HANDLE);
    bgfx::setViewRect(viewID, 0, 0, sizeWidth, sizeHeight);
    bgfx::setViewClear(viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff);
    bgfx::touch(viewID);

	m_filter->setTextures({ lastOutput });
    m_filter->setSubmitViewID(viewID);
    m_filter->submit();
    bgfx::frame();

    this->endOneProcess();

    return true;
}

void AGIPiplineOutputContextWindow::endOneProcess()
{
    AGIContext::sharedContext()->resetValidViewID();

    AGIPiplineTarget::endOneProcess();
}

