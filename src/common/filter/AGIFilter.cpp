//
//  AGIFilter.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilter.h"
#include "filter/core/AGIProgramCacheManager.h"
#include "core/AGIResourceManager.h"
#include "util/bgfxUtils.h"
#include "core/AGIContext.h"

#include "piplineIO/AGIPiplineIO.cpp"


AGIFilter::~AGIFilter() {
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
	m_framebufferImage = nullptr;
}

AGIFilter::AGIFilter()
: m_programKey("default")
, m_program(nullptr)
, m_isPrepared(false)
, m_textures()
, m_submitViewID(0)
, m_framebufferImage()
, m_vertexHandle(BGFX_INVALID_HANDLE)
, m_indexHandle(BGFX_INVALID_HANDLE)
{

}

bool AGIFilter::prepareFilter() {
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

bool AGIFilter::prepareVertex() {
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

bool AGIFilter::prepareProgram() {
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

bool AGIFilter::prepareUniform() {
    if (!m_program->createTexture("s_texture0", 0)) {
        return false;
    }

    return true;
}

bool AGIFilter::setTextures(std::vector<AGIImagePtr> textures) {
    m_textures = textures;

    return true;
}

bool AGIFilter::setSubmitViewID(bgfx::ViewId id) {
    m_submitViewID = id;

    return true;
}

bool AGIFilter::submit() {
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

bool AGIFilter::commitVertex() {
    bgfx::setVertexBuffer(0, m_vertexHandle);
    bgfx::setIndexBuffer(m_indexHandle);

    return true;
}

bool AGIFilter::commitUniform() {
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

bool AGIFilter::commitState() {
    bgfx::setState(0
                   | BGFX_STATE_WRITE_RGB
                   | BGFX_STATE_WRITE_A
                   | BGFX_STATE_MSAA
                   );

    return true;
}

bool AGIFilter::submitProgram() {
    bgfx::submit(m_submitViewID, m_program->getProgramHandle());

    return true;
}

// AGIPiplineNode

// AGIPiplineSource
std::vector<AGIImagePtr> AGIFilter::pullOutputs() {
	if (m_framebufferImage)
	{
		return { m_framebufferImage };
	}

    auto ti = getTargetInputCount();
    std::vector<AGIImagePtr> lastOutputs(ti);
    for (int i = 0; i < ti && i < m_textures.size(); i++) {
        lastOutputs[i] = m_textures[i];
    }
    int sourceIndex = 0;
    for (auto weakSource : m_sources) {
        auto source = weakSource.lock();
	    if (source == nullptr) {
			continue;
	    }
        auto sourceOutputs = source->pullOutputs();
        for (int i = 0; sourceIndex + i < ti && i < source->getSourceOutputCount() && i < sourceOutputs.size(); i++) {
            lastOutputs[sourceIndex + i] = sourceOutputs[i];
        }

        sourceIndex = (sourceIndex + 1) % ti;
    }

    for (auto& lastOutput : lastOutputs)
    {
        if (lastOutput == nullptr)
        {
            return {  };
        }
    }

    auto viewID = AGIContext::sharedContext()->nextValidViewID();
    int sizeWidth = lastOutputs[0]->getSizeWidth();
	int sizeHeight = lastOutputs[0]->getSizeHeight();
	auto framebufferHandle = bgfx::createFrameBuffer(sizeWidth, sizeHeight, bgfx::TextureFormat::Enum::RGBA8);
    bgfx::setViewFrameBuffer(viewID, framebufferHandle);
    bgfx::setViewRect(viewID, 0, 0, sizeWidth, sizeHeight);
    bgfx::setViewClear(viewID, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000ff);
    bgfx::touch(viewID);

    this->prepareFilter();
    this->setTextures(lastOutputs);
    for (int i = 0; i < this->getTargetInputCount(); i++) {
        m_textures[i] = lastOutputs[i];
    }
    this->setSubmitViewID(viewID);
    this->submit();

	m_framebufferImage = std::make_shared<AGIImage>(framebufferHandle, sizeWidth, sizeHeight, bgfx::TextureFormat::Enum::RGBA8);
	return { m_framebufferImage };
}

void AGIFilter::endOneProcess()
{
	m_framebufferImage = nullptr;
	m_textures.clear();

    AGIPiplineNode::endOneProcess();
}
// end of AGIPiplineSource

// AGIPiplineTarget
bool AGIFilter::processTarget() {
    auto outputs = pullOutputs();
    if (outputs.empty()) {
        return false;
    }
    auto textureHandle = outputs.front();
    if (!bgfx::isValid(textureHandle->getGPUTextureHandle())) {
        return false;
    }

    return true;
}
// end of AGIPiplineTarget

// end of AGIPiplineNode

