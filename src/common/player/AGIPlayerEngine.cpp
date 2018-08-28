//
//  AGIPlayerEngine.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPlayerEngine.h"
#include "core/AGIContext.h"
#include "pipline/AGIPiplineGraph.cpp"


AGIPlayerEngine::AGIPlayerEngine()
    : m_piplineGraph{}
    , m_playQueue{"AGIPlayerEngineQueue"}
    , m_mutex{}
    , m_isPaused{false}
    , m_lastFrameDuration{0}
{
    
}

AGIPlayerEngine::~AGIPlayerEngine()
{
    m_piplineGraph = nullptr;
    //m_playQueue;
    //m_mutex;
    m_isPaused = false;
    m_lastFrameDuration = Milliseconds(0);
}

bool AGIPlayerEngine::init(AGIPiplineInputPtr input, AGIPiplineOutputPtr output)
{
    m_piplineGraph = std::make_shared<AGIPiplineGraphPtr::element_type>();
    m_piplineGraph->addSource(input);
    m_piplineGraph->addTarget(output);

    return true;
}

bool AGIPlayerEngine::play()
{
    std::unique_lock<decltype(m_mutex)> lock(m_mutex);
    m_isPaused = false;

    m_playQueue.dispatch([&]()
    {
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            for (int i = 0; i < m_piplineGraph->getTargetCount(); ++i)
            {
				auto target = m_piplineGraph->getTargetAtIndex(i);
				target->processTarget();
            }
        });

        this->handlePlayNextFrame();
    });

    lock.unlock();
    return true;
}

bool AGIPlayerEngine::pause()
{
    std::unique_lock<decltype(m_mutex)> lock(m_mutex);
    m_isPaused = true;

    m_playQueue.syncCancelAllOperation();

    lock.unlock();
    return false;
}

bool AGIPlayerEngine::stop()
{
    this->pause();



    return false;
}

void AGIPlayerEngine::handlePlayNextFrame()
{
    if (m_piplineGraph->getSourcesCount() > 0 && !m_isPaused)
    {
        auto source0 = m_piplineGraph->getSourceAtIndex(0);
        auto input0 = dynamic_cast<AGIPiplineInput*>(source0.get());
        if (input0)
        {
            m_lastFrameDuration = input0->getCurrentFrameDuration();
        }
        else
        {
            m_lastFrameDuration = Milliseconds(1000 / 30);
        }
        // 显示当前帧
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            bgfx::frame();
            for (int i = 0; i < m_piplineGraph->getTargetCount(); ++i)
            {
                auto target = m_piplineGraph->getTargetAtIndex(i);
                target->endOneProcess();
            }
        });

        // 预渲染下一帧，并确定花费的时间
        auto beginTime = std::chrono::steady_clock::now();
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            for (int i = 0; i < m_piplineGraph->getTargetCount(); ++i)
            {
                auto target = m_piplineGraph->getTargetAtIndex(i);
                target->processTarget();
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

        m_playQueue.dispatch([&]()
        {
            this->handlePlayNextFrame();
        });
    }
}

