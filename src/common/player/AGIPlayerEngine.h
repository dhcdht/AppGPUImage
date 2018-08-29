//
//  AGIPlayerEngine.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPlayerEngine_h
#define AGIPlayerEngine_h

#include <iostream>
#include "IO/input/AGIPiplineInput.h"
#include "IO/output/AGIPiplineOutput.h"
#include "filter/core/AGIFilterGraph.h"
#include "core/AGIDispatchQueue.h"


class AGIPlayerEngine
{
public:
    AGIPlayerEngine();
    virtual ~AGIPlayerEngine();

    bool init(AGIPiplineInputPtr input, AGIPiplineOutputPtr output);
    bool init(AGIFilterGraphPtr graph);

public:
    bool play();
    bool pause();
    bool stop();

private:
    void handlePlayNextFrame();

private:
    AGIFilterGraphPtr m_filterGraph;

    AGIDispatchQueue m_playQueue;
    std::mutex m_mutex;

    bool m_isPaused;
    Milliseconds m_lastFrameDuration;
};


#endif /* AGIPlayerEngine_h */
