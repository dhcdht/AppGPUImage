//
//  AGIFilter.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilter.h"
#include "core/AGIContext.h"
#include "pipline/AGIPipline.cpp"


AGIFilter::AGIFilter()
    : m_textures{}
    , m_submitViewID{0}
    , m_framebufferImage{}
{
    
}

AGIFilter::~AGIFilter()
{
    m_textures.clear();
    m_submitViewID = 0;
    m_framebufferImage = nullptr;
}

bool AGIFilter::setTextures(std::vector<AGIImagePtr> textures)
{
    m_textures = textures;

    return true;
}

bool AGIFilter::setSubmitViewID(bgfx::ViewId id)
{
    m_submitViewID = id;

    return true;
}

std::vector<std::shared_ptr<AGIImage>> AGIFilter::pullOutputs()
{
    if (m_framebufferImage)
    {
        return { m_framebufferImage };
    }

    auto ti = getTargetInputCount();
    std::vector<AGIImagePtr> lastOutputs(ti);
    for (int i = 0; i < ti && i < m_textures.size(); i++) {
        lastOutputs[i] = m_textures[i];
    }
    int sourceIndex = 0;
    for (auto weakSource : m_sources) {
        auto source = weakSource.lock();
        if (source == nullptr) {
            continue;
        }
        auto sourceOutputs = source->pullOutputs();
        for (int i = 0; sourceIndex + i < ti && i < source->getSourceOutputCount() && i < sourceOutputs.size(); i++) {
            lastOutputs[sourceIndex + i] = sourceOutputs[i];
        }

        sourceIndex = (sourceIndex + 1) % ti;
    }

    for (auto& lastOutput : lastOutputs)
    {
        if (lastOutput == nullptr)
        {
            return {  };
        }
    }

    auto viewID = AGIContext::sharedContext()->nextValidViewID();
    int sizeWidth = lastOutputs[0]->getSizeWidth();
    int sizeHeight = lastOutputs[0]->getSizeHeight();
    auto framebufferHandle = bgfx::createFrameBuffer(sizeWidth, sizeHeight, bgfx::TextureFormat::Enum::RGBA8);
    bgfx::setViewFrameBuffer(viewID, framebufferHandle);
    bgfx::setViewRect(viewID, 0, 0, sizeWidth, sizeHeight);
    bgfx::setViewClear(viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff);
    bgfx::touch(viewID);

    this->prepareFilter();
    this->setTextures(lastOutputs);
    for (int i = 0; i < this->getTargetInputCount(); i++) {
        m_textures[i] = lastOutputs[i];
    }
    this->setSubmitViewID(viewID);
    this->submitFilter();

    m_framebufferImage = std::make_shared<AGIImage>(framebufferHandle, sizeWidth, sizeHeight, bgfx::TextureFormat::Enum::RGBA8);
    return { m_framebufferImage };
}

void AGIFilter::endOneProcess()
{
    m_framebufferImage = nullptr;
    m_textures.clear();

    AGIPiplineNode::endOneProcess();
}

bool AGIFilter::processTarget()
{
    auto outputs = this->pullOutputs();
    if (outputs.empty()) {
        return false;
    }
    auto textureHandle = outputs.front();
    if (!bgfx::isValid(textureHandle->getGPUTextureHandle())) {
        return false;
    }

    return true;
}

