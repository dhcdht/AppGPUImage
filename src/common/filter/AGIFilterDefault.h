//
//  AGIFilterDefault.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterDefault_h
#define AGIFilterDefault_h

#include "core/AGIFilter.h"
#include "core/AGIProgram.h"


class AGIFilterDefault : public AGIFilter {
public:
    AGIFilterDefault();
    ~AGIFilterDefault() override;

public:
    bool prepareFilter() override;
    bool submitFilter() override;

public:
    int getSourceOutputCount() override {
        return 1;
    };
    int getTargetInputCount() override {
        return 1;
    };

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
private:
    bgfx::VertexBufferHandle m_vertexHandle;
    bgfx::IndexBufferHandle m_indexHandle;
};


#endif /* AGIFilterDefault_h */
