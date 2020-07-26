//
//  AGIPlayerEngine.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPlayerEngine.h"
#include "core/AGIContext.h"
#include "IO/AGIImageConnection.h"


AGIPlayerEngine::AGIPlayerEngine()
    : m_playQueue{"AGIPlayerEngineQueue"}
    , m_playMutex{}
    , m_playable(nullptr)
    , m_output(nullptr)
    , m_isPaused{false}
    , m_lastFrameTimestamp{0}
    , m_lastFrameDuration{0}
{
    
}

AGIPlayerEngine::~AGIPlayerEngine()
{
    std::unique_lock<decltype(m_playMutex)> lock(m_playMutex);

    m_playQueue.syncCancelAllOperation();
    //m_playMutex;
    m_playable = nullptr;
    m_output = nullptr;
    m_isPaused = false;
    m_lastFrameTimestamp = Milliseconds(0);
    m_lastFrameDuration = Milliseconds(0);
}

bool AGIPlayerEngine::init(AGIPlayableInterfacePtr playable, AGIPiplineOutputPtr output)
{
    m_playable = playable;
    m_output = output;

    return true;
}

bool AGIPlayerEngine::play()
{
    std::unique_lock<decltype(m_playMutex)> lock(m_playMutex);
    m_isPaused = false;

    m_playQueue.dispatch([&]()
    {
        auto beginTime = Milliseconds(0);
        auto ioImage = m_playable->getTargetIOImagePtr();
        ioImage->syncSeekToTime(beginTime);
        auto filterGraph = m_playable->getFilterGraphForTime(beginTime);

        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            auto lock = filterGraph->lockGuardGraph();
            if (m_output) {
                auto connection = std::make_shared<AGIImageConnection>(ioImage, m_output);
                m_output->processTarget();
            } else {
                for (int i = 0; i < filterGraph->getGraphTargetCount(); ++i)
                {
                    auto target = filterGraph->getGraphTargetAtIndex(i);
                    target->processTarget();
                }
            }
        });

        m_lastFrameTimestamp = ioImage->getCurrentFrameTime();
        m_lastFrameDuration = ioImage->getCurrentFrameDuration();

        this->handlePlayNextFrame();
    });

    lock.unlock();
    return true;
}

bool AGIPlayerEngine::pause()
{
    std::unique_lock<decltype(m_playMutex)> lock(m_playMutex);
    m_isPaused = true;

    m_playQueue.syncCancelAllOperation();

    lock.unlock();
    return false;
}

bool AGIPlayerEngine::stop()
{
    this->pause();

    // todo: stop logic

    return false;
}

void AGIPlayerEngine::handlePlayNextFrame()
{
    if (!m_isPaused)
    {
        auto ioImage = m_playable->getTargetIOImagePtr();

        // 显示当前帧
        auto filterGraph = m_playable->getFilterGraphForTime(m_lastFrameTimestamp);
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            bgfx::frame();

            auto lock = filterGraph->lockGuardGraph();
            auto output = m_output;
            if (output) {
                auto connection = std::make_shared<AGIImageConnection>(ioImage, output);
                output->endOneProcess();
            } else {
                for (int i = 0; i < filterGraph->getGraphTargetCount(); ++i)
                {
                    auto target = filterGraph->getGraphTargetAtIndex(i);
                    target->endOneProcess();
                }
            }
        });

        auto nextTimestamp = m_lastFrameTimestamp + m_lastFrameDuration;
        auto nextFilterGraph = m_playable->getFilterGraphForTime(nextTimestamp);

        // 预渲染下一帧，并确定花费的时间
        auto beginTime = std::chrono::steady_clock::now();
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            auto lock = nextFilterGraph->lockGuardGraph();
            if (m_output) {
                auto connection = std::make_shared<AGIImageConnection>(ioImage, m_output);
                m_output->processTarget();
            } else {
                for (int i = 0; i < nextFilterGraph->getGraphTargetCount(); ++i)
                {
                    auto target = nextFilterGraph->getGraphTargetAtIndex(i);
                    target->processTarget();
                }
            }
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

        // 变更到显示下一帧的相关环境
        m_lastFrameTimestamp = ioImage->getCurrentFrameTime();
        m_lastFrameDuration = ioImage->getCurrentFrameDuration();

        m_playQueue.dispatch([&]()
        {
            this->handlePlayNextFrame();
        });
    }
}

