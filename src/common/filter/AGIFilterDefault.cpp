//
//  AGIFilter.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterDefault.h"
#include "filter/core/AGIProgramCacheManager.h"
#include "core/AGIResourceManager.h"
#include "util/bgfxUtils.h"
#include "core/AGIContext.h"


AGIFilterDefault::AGIFilterDefault()
    : m_programKey("default")
    , m_program(nullptr)
    , m_isPrepared(false)
    , m_vertexHandle(BGFX_INVALID_HANDLE)
    , m_indexHandle(BGFX_INVALID_HANDLE)
{

}

AGIFilterDefault::~AGIFilterDefault() {
    if (m_program != nullptr) {
        m_program = nullptr;
    }
    if (bgfx::isValid(m_indexHandle)) {
        bgfx::destroy(m_indexHandle);
        m_indexHandle = BGFX_INVALID_HANDLE;
    }
    if (bgfx::isValid(m_vertexHandle)) {
        bgfx::destroy(m_vertexHandle);
        m_vertexHandle = BGFX_INVALID_HANDLE;
    }
}

bool AGIFilterDefault::prepareFilter() {
    if (m_isPrepared) {
        return true;
    }

    if (!prepareVertex()) {
        return false;
    }
    if (!prepareProgram()) {
        return false;
    }
    if (!prepareUniform()) {
        return false;
    }

    m_isPrepared = true;

    return true;
}

bool AGIFilterDefault::prepareVertex() {
    static const float vertexs[4][4] = {
        {-1.0,  -1.0,   0,  1},
        {1.0,   1.0,    1,  0},
        {-1.0,  1.0,    0,  0},
        {1.0,   -1.0,   1,  1},
    };
    bgfx::VertexDecl vertexsDecl;
    vertexsDecl
    .begin()
    .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
    .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    .end();
    m_vertexHandle = bgfx::createVertexBuffer(bgfx::makeRef(vertexs, sizeof(vertexs)), vertexsDecl);
    if (!bgfx::isValid(m_vertexHandle)) {
        return false;
    }

    static const uint16_t indices[6] = {
        0, 1, 2,
        0, 3, 1,
    };
    m_indexHandle = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
    if (!bgfx::isValid(m_indexHandle)) {
        return false;
    }

    return true;
}

bool AGIFilterDefault::prepareProgram() {
    if (m_isPrepared) {
        return true;
    }

    std::string key = m_programKey;

    auto program = AGIProgramCacheManager::sharedManager()->getProgram(key);
    if (program == nullptr) {
        auto bundle = AGIResourceManager::sharedBundle();
        auto vshPath = bundle->getBundleResourcePath("shaders/" + m_programKey + "_vs.sc");
        auto fshPath = bundle->getBundleResourcePath("shaders/" + m_programKey + "_fs.sc");
        auto vsh = bgfxUtils::createShader(bgfxUtils::ShaderType::vertex, vshPath.c_str());
        auto fsh = bgfxUtils::createShader(bgfxUtils::ShaderType::fragment, fshPath.c_str());
        auto programHandle = bgfx::createProgram(vsh, fsh);

        program = std::make_shared<AGIProgram>(vsh, fsh, programHandle);

        AGIProgramCacheManager::sharedManager()->setProgram(key, program);
    }
    if (program == nullptr) {
        return false;
    }

    m_program = program;
    return true;
}

bool AGIFilterDefault::prepareUniform() {
    if (!m_program->createTexture("s_texture0", 0)) {
        return false;
    }

    return true;
}

bool AGIFilterDefault::submitFilter() {
    if (!commitVertex()) {
        return false;
    }
    if (!commitUniform()) {
        return false;
    }
    if (!commitState()) {
        return false;
    }
    if (!submitProgram()) {
        return false;
    }

    return true;
}

bool AGIFilterDefault::commitVertex() {
    bgfx::setVertexBuffer(0, m_vertexHandle);
    bgfx::setIndexBuffer(m_indexHandle);

    return true;
}

bool AGIFilterDefault::commitUniform() {
    if (m_textures[0] == nullptr) {
        return false;
    }
	auto textureHandle = m_textures[0]->getGPUTextureHandle();
	if (!bgfx::isValid(textureHandle)) {
		return false;
	}

    if (!m_program->setTexture("s_texture0", textureHandle)) {
        return false;
    }

    return true;
}

bool AGIFilterDefault::commitState() {
    bgfx::setState(0
                   | BGFX_STATE_WRITE_RGB
                   | BGFX_STATE_WRITE_A
                   | BGFX_STATE_MSAA
                   );

    return true;
}

bool AGIFilterDefault::submitProgram() {
    bgfx::submit(m_submitViewID, m_program->getProgramHandle());

    return true;
}

