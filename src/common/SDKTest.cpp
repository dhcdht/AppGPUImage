//
//  SDKTest.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "SDKTest.h"
#include "util/bgfxUtils.h"
#include "core/AGIContext.h"
#include <bx/bx.h>
#include <bx/file.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>
#include "core/AGIResourceManager.h"
#include <bx/math.h>
#include "filter/AGIFilterDefault.h"
#include "filter/AGIFilterTransform.h"
#include "filter/AGIFilterGrayscale.h"
#include "IO/input/AGIPiplineInputPicture.h"
#include "IO/output/AGIPiplineOutputContextWindow.h"
#include "filter/AGIFilterAlphaBlend.h"
#include "filter/AGIFilterSobelEdgeDetection.h"
#include "IO/input/AGIPiplineInputFFmpegReader.h"
#include "filter/AGIFilterContentMode.h"
#include "timeline/AGITimeline.h"
#include "player/AGIPlayerEngine.h"
#include "IO/AGIPiplineIOImage.h"
#include "filter/core/AGIFilterSetting.hpp"
#include "filter/core/AGIFilterSetting.cpp"


SDKTest::~SDKTest() {

}

SDKTest::SDKTest() {

}

void SDKTest::init(void *window, int sizeWidth, int sizeHeight) {
	AGIContext::sharedContext()->getVideoProcessQueue()->dispatch([=]() {
		AGIContext::sharedContext()->init(window, sizeWidth, sizeHeight);
	});
}

static void testFiltetPlayMovie(std::shared_ptr<AGIPiplineInput> input0)
{
    static uint64_t kOnePartMilliSeconds = 3 * 1000;

	auto input = std::make_shared<AGIFilterContentMode>();
	input0->addTarget(input);

    auto output = std::make_shared<AGIPiplineOutputContextWindow>();
    output->init();

	input->setContentMode(AGIFilterContentMode::ContentMode::AspectFit);

	auto currentTimeMilliSeconds = input0->getCurrentFrameTime();
	int64_t currentTime = currentTimeMilliSeconds.count();
    if (currentTime < kOnePartMilliSeconds)
    {
        auto filter = std::make_shared<AGIFilterSobelEdgeDetection>();

        auto filter3 = std::make_shared<AGIFilterAlphaBlend>();
        double progress = (double)(currentTime) / (double)kOnePartMilliSeconds;
        filter3->setMix(progress);

        input->addTarget(filter);
        input->addTarget(filter3);
        filter->addTarget(filter3);
        filter3->addTarget(output);

        output->processTarget();
    }
    else if (currentTime < kOnePartMilliSeconds * 2)
    {
        auto filter = std::make_shared<AGIFilterGrayscale>();
        auto filter2 = std::make_shared<AGIFilterTransform>();

        double progress = (double)(currentTime - kOnePartMilliSeconds) / (double)kOnePartMilliSeconds;
        filter->setProgress(progress);

        float transform[4][4] = { 0 };
        bx::mtxRotateXYZ((float*)transform, 0.0, 0.0, (bx::kPi / 6)*progress);
        filter2->setTransform(transform);

        input->addTarget(filter);
        filter->addTarget(filter2);
        filter2->addTarget(output);

        output->processTarget();
    }
    else if (currentTime < kOnePartMilliSeconds * 3)
    {
        auto filter = std::make_shared<AGIFilterGrayscale>();
        auto filter2 = std::make_shared<AGIFilterTransform>();

        filter->setProgress(1);

        float transform[4][4] = { 0 };
        bx::mtxRotateXYZ((float*)transform, 0.0, 0.0, (bx::kPi / 6)*1);
        filter2->setTransform(transform);

        auto filter3 = std::make_shared<AGIFilterAlphaBlend>();
        double progress = (double)(currentTime - kOnePartMilliSeconds*2) / (double)kOnePartMilliSeconds;
        filter3->setMix(progress);

        input->addTarget(filter);
        filter->addTarget(filter2);
        filter2->addTarget(filter3);
        input->addTarget(filter3);
        filter3->addTarget(output);

        output->processTarget();
    }
    else
    {
		//auto filter = std::make_shared<AGIFilterOpenCVBackgroundDetect>();
		//static  auto filter = std::make_shared<AGIFilterOpenCVFaceDetect>();

		//input->addTarget(filter);
		//filter->addTarget(output);

		input->addTarget(output);

	    if (!output->processTarget())
	    {
			if (input0->syncSeekToTime(Milliseconds(0)))
			{
				currentTime = 0;
				return;
			}
	    }
    }
}

static AGIPlayerEngine kPlayerEngine;
static AGIFilterGraphPtr kFilterGraph;
void SDKTest::test_playerEngine(const char* filePath)
{
	auto input = std::make_shared<AGIPiplineInputFFmpegReader>();
	input->init(filePath);
	auto contentMode = std::make_shared<AGIFilterContentMode>();
	contentMode->setContentMode(AGIFilterContentMode::AspectFit);
	auto output = std::make_shared<AGIPiplineOutputContextWindow>();
	output->init();

	input->addTarget(contentMode);
	contentMode->addTarget(output);

	kFilterGraph = std::make_shared<AGIFilterGraph>();
	kFilterGraph->addSource(input);
	kFilterGraph->addTarget(output);

	kPlayerEngine.init(kFilterGraph);
	kPlayerEngine.play();
}

void SDKTest::test_pauseEngine()
{
	kPlayerEngine.pause();
}

void SDKTest::test_changeFilter()
{
	auto lock = kFilterGraph->lockGuardGraph();

    auto output0 = kFilterGraph->getOutputAtIndex(0);
	if (output0)
	{
		auto weakSource = output0->getSources()[0];
		auto source = weakSource.lock();
		if (source != nullptr)
		{
			source->removeTarget(output0);

			auto filter = std::make_shared<AGIFilterGrayscale>();
			source->addTarget(filter);
			filter->addTarget(output0);

			//auto setting = AGIFilterSetting<float>();
			//auto setGrayProcessFunc = std::bind(&AGIFilterGrayscale::setProgress, filter, std::placeholders::_1);
			//setting.init(setGrayProcessFunc);

			//setting.setValueForTime(0.3, 0.1);
			//setting.setValueForTime(0.5, 0.5);
			//setting.setValueForTime(0.8, 0.9);
			//setting.setValueForTime(1.0, 1.0);

			//auto value = setting.getValueForTime(0.0);
			//value = setting.getValueForTime(0.2);
			//value = setting.getValueForTime(0.4);
			//value = setting.getValueForTime(0.5);
			//value = setting.getValueForTime(0.99);

			//setting.doSetFuncForTime(0.7);
		}
	}
}

void SDKTest::test_timeline(const char *filePath)
{
	//auto timeline = std::make_shared<AGITimeline>();

	//timeline->addVideoTrack();
	//auto clip = std::make_shared<AGIClip>();
	//clip->init(filePath);
	//timeline->addClipToVideoTrack(clip, 0);

	//auto videoTrack = timeline->getVideoTrackAtIndex(0);
	//auto beginTime = videoTrack->getBeginTime();
	//auto endTime = videoTrack->getEndTime();

	auto clip = std::make_shared<AGIClip>();
	clip->init(filePath);

	auto ioImage = clip->getFilterGraphOutput();
	auto output = std::make_shared<AGIPiplineOutputContextWindow>();
	output->init();

	ioImage->addTarget(output);

	auto outputGraph = std::make_shared<AGIFilterGraph>();
	outputGraph->addSource(ioImage);
	outputGraph->addTarget(output);

	kPlayerEngine.init(outputGraph);
	kPlayerEngine.play();
}

