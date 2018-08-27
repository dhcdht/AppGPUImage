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
#include "core/AGIDispatchQueue.h"


class AGIPlayerEngine
{
public:
    AGIPlayerEngine();
    virtual ~AGIPlayerEngine();

    bool init(AGIPiplineInputPtr input, AGIPiplineOutputPtr output);

public:
    bool play();
    bool pause();
    bool stop();

private:
    void handlePlayNextFrame();

private:
    AGIPiplineInputPtr m_playInput;
    AGIPiplineOutputPtr m_playOutput;

    AGIDispatchQueue m_playQueue;

    Milliseconds m_lastFrameDuration;
};


#endif /* AGIPlayerEngine_h */
