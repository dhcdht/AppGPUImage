//
//  AGIFilterSobelEdgeDetection.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/5.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterSobelEdgeDetection.h"


AGIFilterSobelEdgeDetection::AGIFilterSobelEdgeDetection()
: m_pixelWidth(1.0)
, m_pixelHeight(1.0)
, m_edgeStrength(1.0)
{
    m_programKey = "sobelEdgeDetection";
}

AGIFilterSobelEdgeDetection::~AGIFilterSobelEdgeDetection()
{

}

bool AGIFilterSobelEdgeDetection::setEdgePixelWidth(float pixelWidth)
{
    m_pixelWidth = pixelWidth;

    return true;
}

bool AGIFilterSobelEdgeDetection::setEdgePixelHeight(float pixelHeight)
{
    m_pixelHeight = pixelHeight;

    return true;
}

bool AGIFilterSobelEdgeDetection::setEdgeStrength(float edgeStrength)
{
    m_edgeStrength = edgeStrength;

    return true;
}

bool AGIFilterSobelEdgeDetection::prepareUniform()
{
    if (!AGIFilter::prepareUniform()) {
        return false;
    }

    if (!m_program->createUniform("u_pixelUniform", bgfx::UniformType::Enum::Vec4)) {
        return false;
    }

    return true;
}

bool AGIFilterSobelEdgeDetection::commitUniform()
{
    if (!AGIFilter::commitUniform()) {
        return false;
    }

    float pixels[4] = {m_pixelWidth, m_pixelHeight, m_edgeStrength, 0.0};
    if (!m_program->setUniformValue("u_pixelUniform", pixels)) {
        return false;
    }

    return true;
}

