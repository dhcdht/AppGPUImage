//
//  AGIFilterTwoPass.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/5.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterTwoPass.h"


AGIFilterTwoPass::AGIFilterTwoPass()
{

}

AGIFilterTwoPass::~AGIFilterTwoPass()
{

}

bool AGIFilterTwoPass::prepareUniform()
{
    if (!AGIFilter::prepareUniform()) {
        return false;
    }

    if (!m_program->createTexture("s_texture1", 1)) {
        return false;
    }

    return true;
}

bool AGIFilterTwoPass::commitUniform()
{
    if (!AGIFilter::commitUniform()) {
        return false;
    }

    if (m_textures[1] == nullptr) {
        return false;
    }
	auto textureHandle = m_textures[1]->getGPUTextureHandle();
	if (!bgfx::isValid(textureHandle)) {
		return false;
	}
    if (!m_program->setTexture("s_texture1", textureHandle)) {
        return false;
    }

    return true;
}

