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
{
    
}

AGIPlayerEngine::~AGIPlayerEngine()
{
    m_playInput = nullptr;
    m_playOutput = nullptr;
}

bool AGIPlayerEngine::init(AGIPiplineInputPtr input, AgiPiplineOutputPtr output)
{
    m_playInput = input;
    m_playOutput = output;

    return true;
}

bool AGIPlayerEngine::play()
{
    AGIContext::sharedContext()->getVideoProcessQueue()->dispatch([&]() {
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
        auto beginTime = std::chrono::steady_clock::now();

        m_playOutput->processTarget();

        auto endTime = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<Milliseconds>(endTime - beginTime);

        auto frameRate = m_playInput->getPreferFrameRate();
        auto nextTime = Milliseconds((int)(1000.0 / frameRate)) - duration - Milliseconds(1);
        std::this_thread::sleep_for(nextTime);
        AGIContext::sharedContext()->getVideoProcessQueue()->dispatch([&]() {
            this->handlePlayNextFrame();
        });
    }
}

