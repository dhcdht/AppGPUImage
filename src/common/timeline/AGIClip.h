//
//  AGIClip.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIClip_h
#define AGIClip_h

#include <iostream>
#include <chrono>
#include "IO/input/AGIPiplineInput.h"
#include "util/bgfxUtils.h"
#include "IO/AGIPiplineIOImage.h"
#include "filter/core/AGIFilterGraph.h"


class AGIClip : public AGIPiplineInput
{
public:
	AGIClip();
	virtual ~AGIClip();

	bool init(const std::string filePath) override;

public:
	Milliseconds getReaderBeginTime();
	bool setReaderBeginTime(Milliseconds readerBeginTime);
	Milliseconds getReaderEndTime();
	bool setReaderEndTime(Milliseconds readerEndTime);
	Milliseconds getTrackBeginTime();
	bool setTrackBeginTime(Milliseconds trackBeginTime);
	Milliseconds getTrackEndTime();
	bool setTrackEndTime(Milliseconds trackEndTime);

	AGIFilterGraphPtr getFilterGraph();
	AGIPiplineIOImagePtr getFilterGraphOutput();

	//region AGIPiplineInput
public:
	Milliseconds getDuration() override;
	int getPreferFrameRate() override;
	bool syncSeekToTime(Milliseconds time) override;
	Milliseconds getCurrentFrameTime() override;
	Milliseconds getCurrentFrameDuration() override;

	//region AGIPiplineSource
	int getSourceOutputCount() override { return 1; };
	std::vector<AGIImagePtr> pullOutputs() override;
	void endOneProcess() override;
	//endregion AGIPiplineSource

	//endregion AGIPiplineInput

private:
	AGIPiplineInputPtr m_input;
	AGIPiplineIOImagePtr m_output;
	AGIFilterGraphPtr m_filterGraph;

	Milliseconds m_readerBeginTime;
	Milliseconds m_readerEndTime;
	Milliseconds m_trackBeginTime;
	Milliseconds m_trackEndTime;
};
typedef std::shared_ptr<AGIClip> AGIClipPtr;


#endif /* AGIClip_h */