//
//  AGIFilter.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilter_h
#define AGIFilter_h

#include "pipline/AGIPipline.hpp"
#include "IO/AGIImage.h"


class AGIFilter : public AGIPiplineNode<AGIImagePtr, AGIImagePtr>
{
public:
    AGIFilter();
    virtual ~AGIFilter();

public:
    virtual bool prepareFilter() = 0;
    bool setTextures(std::vector<AGIImagePtr> textures);
    bool setSubmitViewID(bgfx::ViewId id);
    virtual bool submitFilter() = 0;

protected:
    std::vector<AGIImagePtr> m_textures;
    bgfx::ViewId m_submitViewID;

    //region AGIPiplineNode

    //region AGIPiplineSource
public:
    std::vector<std::shared_ptr<AGIImage>> pullOutputs() override;
    void endOneProcess() override;
    //endregion AGIPiplineSource

    //region AGIPiplineTarget
public:
    bool processTarget() override;
    //endregion AGIPiplineTarget

    //endregion AGIPiplineNode

protected:
    AGIImagePtr m_framebufferImage;
};


#endif /* AGIFilter_h */
