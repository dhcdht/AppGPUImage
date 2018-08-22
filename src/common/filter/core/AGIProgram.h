//
//  AGIProgram.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/27.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIProgram_h
#define AGIProgram_h

#include <iostream>
#include <memory>
#include <bgfx/bgfx.h>
#include <unordered_map>


class AGIProgram : public std::enable_shared_from_this<AGIProgram> {
public:
    AGIProgram(bgfx::ShaderHandle vsh, bgfx::ShaderHandle fsh, bgfx::ProgramHandle program);
    virtual ~AGIProgram();

public:
    bool createUniform(std::string name, bgfx::UniformType::Enum type);
    bool setUniformValue(std::string name, const void* value);

    bool createTexture(std::string name, uint8_t stage);
    bool setTexture(std::string name, bgfx::TextureHandle textureHandle);

    bgfx::ProgramHandle getProgramHandle();

private:
    bgfx::ShaderHandle m_vertexShaderHandle;
    bgfx::ShaderHandle m_fragmentShaderHandle;
    bgfx::ProgramHandle m_programHandle;

    struct Uniform {
        bgfx::UniformHandle handle = BGFX_INVALID_HANDLE;
    };
    std::unordered_map<std::string, Uniform> m_uniformMap;
    struct TextureUniform {
        uint8_t stage = 0;
        bgfx::UniformHandle handle = BGFX_INVALID_HANDLE;
    };
    std::unordered_map<std::string, TextureUniform> m_textureMap;
};
typedef std::shared_ptr<AGIProgram> AGIProgramPtr;


#endif /* AGIProgram_h */
