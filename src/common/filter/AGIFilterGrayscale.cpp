//
//  AGIFilterGrayscale.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterGrayscale.h"


AGIFilterGrayscale::AGIFilterGrayscale()
: m_progress(1)
{
    m_programKey = "grayscale";
}

AGIFilterGrayscale::~AGIFilterGrayscale()
{

}

bool AGIFilterGrayscale::setProgress(float progress)
{
    m_progress = progress;

    return true;
}

bool AGIFilterGrayscale::prepareUniform()
{
    if (!AGIFilterDefault::prepareUniform()) {
        return false;
    }

    if (!m_program->createUniform("u_progress", bgfx::UniformType::Vec4)) {
        return false;
    }

    return true;
}

bool AGIFilterGrayscale::commitUniform()
{
    float progressUniform[4] = {m_progress, m_progress, m_progress, m_progress};
    if (!m_program->setUniformValue("u_progress", progressUniform)) {
        return false;
    }

    return AGIFilterDefault::commitUniform();
}

