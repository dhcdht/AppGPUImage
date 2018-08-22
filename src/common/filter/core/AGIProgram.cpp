//
//  AGIProgram.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/27.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIProgram.h"


AGIProgram::AGIProgram(bgfx::ShaderHandle vsh, bgfx::ShaderHandle fsh, bgfx::ProgramHandle program)
: m_vertexShaderHandle(vsh)
, m_fragmentShaderHandle(fsh)
, m_programHandle(program)
, m_uniformMap()
, m_textureMap()
{

}

AGIProgram::~AGIProgram()
{
    if (bgfx::isValid(m_vertexShaderHandle)) {
        bgfx::destroy(m_vertexShaderHandle);
        m_vertexShaderHandle = BGFX_INVALID_HANDLE;
    }
    if (bgfx::isValid(m_fragmentShaderHandle)) {
        bgfx::destroy(m_fragmentShaderHandle);
        m_fragmentShaderHandle = BGFX_INVALID_HANDLE;
    }
    if (bgfx::isValid(m_programHandle)) {
        bgfx::destroy(m_programHandle);
        m_programHandle = BGFX_INVALID_HANDLE;
    }

    for (auto& uniform : m_uniformMap) {
        if (bgfx::isValid(uniform.second.handle)) {
            bgfx::destroy(uniform.second.handle);
        }
    }
    m_uniformMap.clear();

    for (auto& texture : m_textureMap) {
        if (bgfx::isValid(texture.second.handle)) {
            bgfx::destroy(texture.second.handle);
        }
    }
    m_textureMap.clear();
}

bool AGIProgram::createUniform(std::string name, bgfx::UniformType::Enum type)
{
    auto findIt = m_uniformMap.find(name);
    if (findIt != m_uniformMap.end()) {
        return true;
    }

    auto uniformHandle = bgfx::createUniform(name.c_str(), type);
    if (!bgfx::isValid(uniformHandle)) {
        return false;
    }
    Uniform uniform = {uniformHandle};

    m_uniformMap[name] = uniform;

    return true;
}
bool AGIProgram::setUniformValue(std::string name, const void* value)
{
    auto uniform = m_uniformMap[name];
    if (!bgfx::isValid(uniform.handle)) {
        return false;
    }

    bgfx::setUniform(uniform.handle, value);

    return true;
}

bool AGIProgram::createTexture(std::string name, uint8_t stage)
{
    auto findIt = m_textureMap.find(name);
    if (findIt != m_textureMap.end()) {
        return true;
    }

    auto uniformHandle = bgfx::createUniform(name.c_str(), bgfx::UniformType::Enum::Int1);
    if (!bgfx::isValid(uniformHandle)) {
        return false;
    }
    TextureUniform textureUniform = { stage, uniformHandle };

    m_textureMap[name] = textureUniform;

    return true;
}
bool AGIProgram::setTexture(std::string name, bgfx::TextureHandle textureHandle)
{
    auto textureUniform = m_textureMap[name];
    if (!bgfx::isValid(textureUniform.handle)) {
        return false;
    }

    bgfx::setTexture(textureUniform.stage, textureUniform.handle, textureHandle);

    return true;
}

bgfx::ProgramHandle AGIProgram::getProgramHandle()
{
    return m_programHandle;
}

