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


class CascadeDetectorAdapter : public cv::DetectionBasedTracker::IDetector
{
public:
    CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> classifier)
        : IDetector{}
        , m_classifier{ classifier }
    {}
    ~CascadeDetectorAdapter()
    {
        m_classifier.release();
    }

public:
    void detect(const cv::Mat& image, std::vector<cv::Rect>& objects) override
    {
        m_classifier->detectMultiScale(image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
    }

private:
    cv::Ptr<cv::CascadeClassifier> m_classifier;
};


AGIFilterOpenCVFaceDetect::AGIFilterOpenCVFaceDetect()
    : m_classifier{}
    , m_mainDetector{}
    , m_trackingDetector{}
    , m_tracker{}
{
}

AGIFilterOpenCVFaceDetect::~AGIFilterOpenCVFaceDetect()
{
    m_classifier.release();
    m_mainDetector.release();
    m_trackingDetector.release();
    if (m_tracker != nullptr)
    {
        m_tracker->stop();

        delete m_tracker;
        m_tracker = nullptr;
    }
}

bool AGIFilterOpenCVFaceDetect::prepareFilter()
{
    if (m_isPrepared)
    {
        return true;
    }

    auto faceDetectModelPath = AGIResourceManager::sharedBundle()->getBundleResourcePath("model/lbpcascades/lbpcascade_frontalface.xml");
    m_classifier = cv::makePtr<cv::CascadeClassifier>(faceDetectModelPath);
    if (m_classifier.empty())
    {
        return false;
    }

    m_mainDetector = cv::makePtr<CascadeDetectorAdapter>(m_classifier);
    if (m_mainDetector.empty())
    {
        return false;
    }

    auto cascade = cv::makePtr<cv::CascadeClassifier>(faceDetectModelPath);
    m_trackingDetector = cv::makePtr<CascadeDetectorAdapter>(cascade);
    if (m_trackingDetector.empty())
    {
        return false;
    }

    cv::DetectionBasedTracker::Parameters params;
    m_tracker = new cv::DetectionBasedTracker(m_mainDetector, m_trackingDetector, params);
    if (!m_tracker->run())
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
    std::vector<cv::Rect> faceRects;
    m_tracker->process(grayFrame);
    m_tracker->getObjects(faceRects);

    return AGIFilter::submit();
}

