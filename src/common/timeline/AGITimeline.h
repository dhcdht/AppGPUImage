//
//  AGITimeline.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGITimeline_h
#define AGITimeline_h

#include <iostream>
#include <vector>
#include "AGITrack.h"
#include "AGIClip.h"


class AGITimeline
{
public:
	AGITimeline();
	virtual ~AGITimeline();

	// 项目结构相关操作
public:
	int getVideoTrackCount();
	bool addVideoTrack(AGITrackPtr videoTrack = nullptr);
	AGITrackPtr getVideoTrackAtIndex(int videoTrackIndex);
	bool addClipToVideoTrack(AGIClipPtr clip, int videoTrackIndex);

	// 播放相关操作
public:
	Milliseconds getCurrentPlayTime();

private:
	std::vector<AGITrackPtr> m_videoTracks;
	std::vector<AGITrackPtr> m_audioTracks;

private:
	Milliseconds m_curPlayTime;
};
typedef std::shared_ptr<AGITimeline> AGITimelinePtr;


#endif /* AGITimeline_h */
