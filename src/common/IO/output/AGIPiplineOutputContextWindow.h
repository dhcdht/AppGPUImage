//
//  AGIPiplineOutputContextWindow.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineOutputContextWindow_h
#define AGIPiplineOutputContextWindow_h

#include "piplineIO/AGIPiplineIO.hpp"
#include <bgfx/bgfx.h>
#include "filter/AGIFilter.h"
#include "IO/AGIImage.h"


class AGIPiplineOutputContextWindow : public AGIPiplineTarget<AGIImagePtr> {
public:
    AGIPiplineOutputContextWindow();
    virtual ~AGIPiplineOutputContextWindow();

    bool init();

private:
    std::shared_ptr<AGIFilter> m_filter;

    // begin of AGIPiplineTarget
public:
    int getTargetInputCount() override { return 1; };
    bool processTarget() override;
    void endOneProcess() override;
    // end of AGIPiplineTarget
};


#endif /* AGIPiplineOutputContextWindow_h */
