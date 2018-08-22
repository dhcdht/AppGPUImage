//
//  AGITrack.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGITrack.h"


AGITrack::AGITrack()
	: m_clips{}

	, m_curPlayTime{0}
{
	
}

AGITrack::~AGITrack()
{
	m_clips.clear();

	m_curPlayTime = Milliseconds(0);
}

int AGITrack::getClipCount()
{
	return m_clips.size();
}

Milliseconds AGITrack::getBeginTime()
{
	Milliseconds beginTime(0);
	for (auto clip : m_clips)
	{
		if (clip->getTrackBeginTime() > beginTime)
		{
			beginTime = clip->getTrackBeginTime();
		}
	}

	return beginTime;
}

Milliseconds AGITrack::getEndTime()
{
	if (m_clips.size() == 0)
	{
		return Milliseconds(0);
	}

	Milliseconds endTime(0);
	for (auto clip : m_clips)
	{
		if (clip->getTrackEndTime() > endTime)
		{
			endTime = clip->getTrackEndTime();
		}
	}

	return endTime;
}

bool AGITrack::addClip(AGIClipPtr clip)
{
	m_clips.push_back(clip);

	return true;
}

Milliseconds AGITrack::getCurrentPlayTime()
{
	return m_curPlayTime;
}

