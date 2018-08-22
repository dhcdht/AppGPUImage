//
//  AGIFilterAlphaBlend.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterAlphaBlend_h
#define AGIFilterAlphaBlend_h

#include "AGIFilterTwoPass.h"
#include <bgfx/bgfx.h>


class AGIFilterAlphaBlend : public AGIFilterTwoPass {
public:
    AGIFilterAlphaBlend();
    ~AGIFilterAlphaBlend() override;

public:
    bool setMix(float mix);

protected:
    bool prepareUniform() override;
protected:
    bool commitUniform() override;
private:
    float m_mix;
};


#endif /* AGIFilterAlphaBlend_h */
