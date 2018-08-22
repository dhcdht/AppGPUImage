//
//  AGIFilter.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilter_h
#define AGIFilter_h

#include <iostream>
#include <string>
#include <bgfx/bgfx.h>
#include "filter/core/AGIProgram.h"
#include "piplineIO/AGIPiplineIO.hpp"
#include "IO/AGIImage.h"


class AGIFilter : public AGIPiplineNode<AGIImagePtr, AGIImagePtr> {
public:
    virtual ~AGIFilter();
    AGIFilter();

public:
    bool setSubmitViewID(bgfx::ViewId id);
    bool setTextures(std::vector<AGIImagePtr> textures);

public:
    virtual bool prepareFilter();
public:
    virtual bool submit();

public:
    int getSourceOutputCount() override {
        return 1;
    };
    std::vector<AGIImagePtr> pullOutputs() override;
public:
    int getTargetInputCount() override {
        return 1;
    };
    bool processTarget() override;
    void endOneProcess() override;

protected:
    virtual bool prepareVertex();
    virtual bool prepareProgram();
    virtual bool prepareUniform();
protected:
    virtual bool commitVertex();
    virtual bool commitUniform();
    virtual bool commitState();
    virtual bool submitProgram();

protected:
    std::string m_programKey;
    std::shared_ptr<AGIProgram> m_program;
    bool m_isPrepared;
    std::vector<AGIImagePtr> m_textures;
    AGIImagePtr m_framebufferImage;
private:
    bgfx::ViewId m_submitViewID;
    bgfx::VertexBufferHandle m_vertexHandle;
    bgfx::IndexBufferHandle m_indexHandle;
};


#endif /* AGIFilter_h */
