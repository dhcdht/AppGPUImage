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
#include "filter/AGIFilter.h"
#include "filter/AGIFilterTransform.h"
#include "filter/AGIFilterGrayscale.h"
#include "IO/input/AGIPiplineInputPicture.h"
#include "IO/output/AGIPiplineOutputContextWindow.h"
#include "filter/AGIFilterAlphaBlend.h"
#include "filter/AGIFilterSobelEdgeDetection.h"
#include "IO/input/AGIPiplineInputFFmpegReader.h"
#include "filter/AGIFilterContentMode.h"
#include "timeline/AGITimeline.h"
#include "filter/AGIFilterOpenCVBackgroundDetect.h"
#include "filter/AGIFilterOpenCVFaceDetect.h"


SDKTest::~SDKTest() {

}

SDKTest::SDKTest() {

}

void SDKTest::init(void *window, int sizeWidth, int sizeHeight) {
	AGIContext::sharedContext()->getVideoProcessQueue()->dispatch([=]() {
		AGIContext::sharedContext()->init(window, sizeWidth, sizeHeight);
	});
}

void SDKTest::test_timeline(const char *filePath)
{
	auto timeline = std::make_shared<AGITimeline>();

	timeline->addVideoTrack();
	auto clip = std::make_shared<AGIClip>();
	clip->init(filePath);
	timeline->addClipToVideoTrack(clip, 0);

	auto videoTrack = timeline->getVideoTrackAtIndex(0);
	auto beginTime = videoTrack->getBeginTime();
	auto endTime = videoTrack->getEndTime();
}


static void testFiltetPlayMovie(std::shared_ptr<AGIPiplineInput> input0)
{
    static uint64_t kOnePartMilliSeconds = 3 * 1000;

	auto input = std::make_shared<AGIFilterContentMode>();
	input0->addTarget(input);

    auto output = std::make_shared<AGIPiplineOutputContextWindow>();
    output->init();

	input->setContentMode(AGIFilterContentMode::ContentMode::AspectFit);

	auto currentTimeMilliSeconds = input0->getCurrentTime();
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
		auto filter = std::make_shared<AGIFilterOpenCVBackgroundDetect>();
		//static  auto filter = std::make_shared<AGIFilterOpenCVFaceDetect>();

		input->addTarget(filter);
		filter->addTarget(output);

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

static std::shared_ptr<AGIPiplineInput> kInput = nullptr;
void SDKTest::test_openMovieFile(const char* filePath)
{
    kInput = std::make_shared<AGIPiplineInputFFmpegReader>();
    kInput->init(filePath);
	//kInput = std::make_shared<AGIPiplineInputPicture>();

	std::string filePathString = filePath;
	AGIContext::sharedContext()->getVideoProcessQueue()->dispatch([&, filePathString]() {
		//kInput->init(filePathString);
		this->test_playMovieNextFrame();
	});
}

void SDKTest::test_playMovieNextFrame()
{
	if (kInput != nullptr)
	{
        auto begin = std::chrono::steady_clock::now();
        
		kInput->removeAllTarget();
		testFiltetPlayMovie(kInput);
        
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<Milliseconds>(end - begin);

		auto frameRate = kInput->getPreferFrameRate();
        
        auto nextTime = Milliseconds((int)(1000.0/frameRate)) - duration - Milliseconds(1);
        std::this_thread::sleep_for(nextTime);
        AGIContext::sharedContext()->getVideoProcessQueue()->dispatch([&]() {
            this->test_playMovieNextFrame();
        });
	}
}
