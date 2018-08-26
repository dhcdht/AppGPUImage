//
//  AGIPiplineOutputContextWindow.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineOutputContextWindow_h
#define AGIPiplineOutputContextWindow_h

#include "AGIPiplineOutput.h"
#include <bgfx/bgfx.h>
#include "filter/AGIFilterDefault.h"


class AGIPiplineOutputContextWindow : public AGIPiplineOutput {
public:
    AGIPiplineOutputContextWindow();
    virtual ~AGIPiplineOutputContextWindow();

    bool init() override;

private:
    std::shared_ptr<AGIFilterDefault> m_filter;

    // begin of AGIPiplineTarget
public:
    int getTargetInputCount() override { return 1; };
    bool processTarget() override;
    void endOneProcess() override;
    // end of AGIPiplineTarget
};


#endif /* AGIPiplineOutputContextWindow_h */
