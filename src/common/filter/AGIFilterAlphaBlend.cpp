//
//  AGIFilterAlphaBlend.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterAlphaBlend.h"
#include "core/AGIContext.h"


AGIFilterAlphaBlend::AGIFilterAlphaBlend()
: m_mix(0.5)
{
    m_programKey = "alphaBlend";
}

AGIFilterAlphaBlend::~AGIFilterAlphaBlend()
{

}

bool AGIFilterAlphaBlend::prepareUniform()
{
    if (!AGIFilterTwoPass::prepareUniform()) {
        return false;
    }

    if (!m_program->createUniform("u_mixUniform", bgfx::UniformType::Vec4)) {
        return false;
    }

    return true;
}

bool AGIFilterAlphaBlend::setMix(float mix)
{
    m_mix = mix;

    return true;
}

bool AGIFilterAlphaBlend::commitUniform()
{
    if (!AGIFilterTwoPass::commitUniform()) {
        return false;
    }

    float mixUniform[4] = {m_mix, m_mix, m_mix, m_mix};
    if (!m_program->setUniformValue("u_mixUniform", mixUniform)) {
        return false;
    }

    return true;
}

