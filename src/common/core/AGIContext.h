//
//  AGIContext.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIContext_h
#define AGIContext_h

#include <iostream>
#include "AGIResourceManager.h"
#include <bgfx/platform.h>
#include "AGIDispatchQueue.h"


class AGIContext {
public:
    AGIContext();
    virtual ~AGIContext();

    static AGIContext* sharedContext();

public:
    bool init(void *window, int windowSizeWidth, int windowSizeHeight);

    bx::AllocatorI* getDefaultAllocator();

    void setWindowSizeChange(int windowSizeWidth, int windowSizeHeight);
    int getWindowSizeWidth();
    int getWindowSizeHeight();

    bgfx::ViewId getCurValidViewID();
    bgfx::ViewId nextValidViewID();
    bgfx::ViewId resetValidViewID();

	AGIDispatchQueue* getVideoProcessQueue();

private:
    bgfx::PlatformData m_platformData;
    bx::AllocatorI* m_defaultAllocator;
    void *m_window;
    int m_windowSizeWidth;
    int m_windowSizeHeight;
    bgfx::ViewId m_curValidViewID;
	AGIDispatchQueue *m_videoProcessQueue;
};


#endif /* AGIContext_h */
