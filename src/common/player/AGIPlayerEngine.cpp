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
    : m_filterGraph{}
    , m_playQueue{"AGIPlayerEngineQueue"}
    , m_mutex{}
    , m_isPaused{false}
    , m_lastFrameDuration{0}
{
    
}

AGIPlayerEngine::~AGIPlayerEngine()
{
    m_filterGraph = nullptr;
    //m_playQueue;
    //m_mutex;
    m_isPaused = false;
    m_lastFrameDuration = Milliseconds(0);
}

bool AGIPlayerEngine::init(AGIPiplineInputPtr input, AGIPiplineOutputPtr output)
{
    m_filterGraph = std::make_shared<AGIFilterGraph>();
    m_filterGraph->setAttachmentInput(input);
    m_filterGraph->setAttachmentOutput(output);

    return true;
}

bool AGIPlayerEngine::init(AGIFilterGraphPtr graph)
{
    m_filterGraph = graph;

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
            auto lock = m_filterGraph->lockGuardGraph();
            auto output = m_filterGraph->getAttachmentOutput();
            if (output) {
                output->processTarget();
            } else {
                for (int i = 0; i < m_filterGraph->getGraphTargetCount(); ++i)
                {
                    auto target = m_filterGraph->getGraphTargetAtIndex(i);
                    target->processTarget();
                }
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

    // todo: stop logic

    return false;
}

void AGIPlayerEngine::handlePlayNextFrame()
{
    if (!m_isPaused)
    {
        {
            auto lock = m_filterGraph->lockGuardGraph();
            AGIPiplineInputPtr input = m_filterGraph->getAttachmentInput();
            if (input)
            {
                m_lastFrameDuration = input->getCurrentFrameDuration();
            }
            else
            {
                m_lastFrameDuration = Milliseconds(1000 / 30);
            }
        }
        // 显示当前帧
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            bgfx::frame();

            auto lock = m_filterGraph->lockGuardGraph();
            auto output = m_filterGraph->getAttachmentOutput();
            if (output) {
                output->endOneProcess();
            } else {
                for (int i = 0; i < m_filterGraph->getGraphTargetCount(); ++i)
                {
                    auto target = m_filterGraph->getGraphTargetAtIndex(i);
                    target->endOneProcess();
                }
            }
        });

        // 预渲染下一帧，并确定花费的时间
        auto beginTime = std::chrono::steady_clock::now();
        AGIContext::sharedContext()->getVideoProcessQueue()->syncDispatch([&]()
        {
            auto lock = m_filterGraph->lockGuardGraph();
            auto output = m_filterGraph->getAttachmentOutput();
            if (output) {
                output->processTarget();
            } else {
                for (int i = 0; i < m_filterGraph->getGraphTargetCount(); ++i)
                {
                    auto target = m_filterGraph->getGraphTargetAtIndex(i);
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

        m_playQueue.dispatch([&]()
        {
            this->handlePlayNextFrame();
        });
    }
}

