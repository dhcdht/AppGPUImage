//
//  AGIFilterOpenCVFaceDetect.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/20.
//  Copyright © 2018年 董宏昌. All rights reserved.
//
#include "AGIFilterOpenCVFaceDetect.h"
#include "core/AGIResourceManager.h"
#include "util/bgfxUtils.h"
#include "../common_deps/bgfx/src/renderer.h"


AGIFilterOpenCVFaceDetect::AGIFilterOpenCVFaceDetect()
    : m_classifier{}
{
}

AGIFilterOpenCVFaceDetect::~AGIFilterOpenCVFaceDetect()
{
    m_classifier.release();
}

bool AGIFilterOpenCVFaceDetect::prepareFilter()
{
    if (m_isPrepared)
    {
        return true;
    }

    auto faceDetectModelPath = AGIResourceManager::sharedBundle()->getBundleResourcePath("model/haarcascades/haarcascade_frontalface_default.xml");
    m_classifier = cv::makePtr<cv::CascadeClassifier>(faceDetectModelPath);
    if (m_classifier.empty())
    {
        return false;
    }

    return AGIFilter::prepareFilter();
}

bool AGIFilterOpenCVFaceDetect::submit()
{
    if (m_textures.size() < 1)
    {
        return false;
    }

    // 让 gpu 将 m_textures[0] 绘制出来
    bgfx::frame();
    auto videoFrame = m_textures[0];
    auto width = videoFrame->getSizeWidth();
    auto height = videoFrame->getSizeHeight();
    auto cpuBuffer = videoFrame->getCPUBufferFocre();
    cv::Mat frame = bgfxUtils::generateCVMatFromBuffer(cpuBuffer, bgfx::TextureFormat::Enum::RGBA8, width, height);
    cv::Mat grayFrame;
    cv::cvtColor(frame, grayFrame, CV_BGRA2GRAY);
    cv::equalizeHist(grayFrame, grayFrame);
    std::vector<cv::Rect> faceRects;
    m_classifier->detectMultiScale(grayFrame, faceRects);

    return AGIFilter::submit();
}

