//
//  AGITimeline.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGITimeline.h"
#include "util/AGIUtilDefer.h"


AGITimeline::AGITimeline()
	: m_videoTracks{}
	, m_audioTracks{}

	, m_curPlayTime{0}
{
	
}

AGITimeline::~AGITimeline()
{
	m_videoTracks.clear();
	m_audioTracks.clear();

	m_curPlayTime = Milliseconds(0);
}

int AGITimeline::getVideoTrackCount()
{
	return m_videoTracks.size();
}


bool AGITimeline::addVideoTrack(AGITrackPtr videoTrack)
{
	if (nullptr == videoTrack)
	{
		videoTrack = std::make_shared<AGITrack>();
	}
	m_videoTracks.push_back(videoTrack);

	return true;
}

AGITrackPtr AGITimeline::getVideoTrackAtIndex(int videoTrackIndex)
{
	return m_videoTracks[videoTrackIndex];
}

bool AGITimeline::addClipToVideoTrack(AGIClipPtr clip, int videoTrackIndex)
{
	if (videoTrackIndex >= m_videoTracks.size())
	{
		return false;
	}

	auto videoTrack = m_videoTracks[videoTrackIndex];
	return videoTrack->addClip(clip);
}

Milliseconds AGITimeline::getCurrentPlayTime()
{
	return m_curPlayTime;
}

