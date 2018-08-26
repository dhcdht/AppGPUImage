//
//  AGIPiplineInputPicture.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineInputPicture_h
#define AGIPiplineInputPicture_h

#include "IO/input/AGIPiplineInput.h"
#include <bgfx/bgfx.h>


class AGIPiplineInputPicture : public AGIPiplineInput {
public:
    AGIPiplineInputPicture();
    ~AGIPiplineInputPicture() override;

public:
    bool init(bgfx::TextureHandle textureHandle);

    bgfx::TextureHandle getTextureHandle();

    // begin of AGIPiplineInput
public:
    bool init(const std::string filePath) override;
    std::chrono::milliseconds getDuration() override;
    int getPreferFrameRate() override;
    bool syncSeekToTime(Milliseconds time) override;
    Milliseconds getCurrentFrameTime() override;
    Milliseconds getCurrentFrameDuration() override;
    // end of AGIPiplineInput

    // begin of AGIPiplineSource
public:
    int getSourceOutputCount() override;
    std::vector<AGIImagePtr> pullOutputs() override;
    // end of AGIPiplineSource

protected:
    AGIImagePtr m_image;
};


#endif /* AGIPiplineInputPicture_h */
