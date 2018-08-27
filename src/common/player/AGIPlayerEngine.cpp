//
//  AGIPlayerEngine.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPlayerEngine.h"
#include "core/AGIContext.h"


AGIPlayerEngine::AGIPlayerEngine()
    : m_playInput{nullptr}
    , m_playOutput{nullptr}
    , m_playQueue{"AGIPlayerEngineQueue"}
    , m_lastFrameDuration{0}
{
    
}

AGIPlayerEngine::~AGIPlayerEngine()
{
    m_playInput = nullptr;
    m_playOutput = nullptr;
    //m_playQueue;
    m_lastFrameDuration = Milliseconds(0);
}

bool AGIPlayerEngine::init(AGIPiplineInputPtr input, AGIPiplineOutputPtr output)
{
    m_playInput = input;
    m_playOutput = output;

    return true;
}

bool AGIPlayerEngine::play()
{
    m_playQueue.dispatch([&]()
    {
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            m_playOutput->processTarget();
        });

        this->handlePlayNextFrame();
    });

    return true;
}

bool AGIPlayerEngine::pause()
{
    return false;
}

bool AGIPlayerEngine::stop()
{
    return false;
}

void AGIPlayerEngine::handlePlayNextFrame()
{
    if (m_playInput)
    {
        m_lastFrameDuration = m_playInput->getCurrentFrameDuration();
        // 显示当前帧
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            bgfx::frame();
            m_playOutput->endOneProcess();
        });

        // 预渲染下一帧，并确定花费的时间
        auto beginTime = std::chrono::steady_clock::now();
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            m_playOutput->processTarget();
        });
        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<Milliseconds>(endTime - beginTime);

        // 距离下一帧显示还有多少时间
        auto nextTime = m_lastFrameDuration - duration;
        if (nextTime > Milliseconds(0))
        {
            auto nextTime = m_lastFrameDuration - duration;
            std::this_thread::sleep_for(nextTime);
        }

        m_playQueue.dispatch([&]()
        {
            this->handlePlayNextFrame();
        });
    }
}

