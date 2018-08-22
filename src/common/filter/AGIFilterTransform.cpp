//
//  AGIFilterTransform.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterTransform.h"
#include <bgfx/bgfx.h>
#include <bx/math.h>


AGIFilterTransform::AGIFilterTransform()
{
    bx::mtxIdentity((float*)m_transform);

    m_programKey = "transform";
}

AGIFilterTransform::~AGIFilterTransform()
{

}

bool AGIFilterTransform::setTransform(float mat[4][4]) {
    memcpy(m_transform, mat, sizeof(m_transform));

    return true;
}

bool AGIFilterTransform::prepareUniform() {
    if (!AGIFilter::prepareUniform()) {
        return false;
    }

    if (!m_program->createUniform("u_transform", bgfx::UniformType::Mat4)) {
        return false;
    }

    return true;
}

bool AGIFilterTransform::commitUniform() {
    if (!m_program->setUniformValue("u_transform", m_transform)) {
        return false;
    }

    return AGIFilter::commitUniform();
}

