//
//  AGITrack.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/1.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGITrack_h
#define AGITrack_h

#include <iostream>
#include <vector>
#include "AGIClip.h"


class AGITrack
{
public:
	AGITrack();
	virtual ~AGITrack();

	// 项目结构相关操作
public:
	int getClipCount();
	Milliseconds getBeginTime();
	Milliseconds getEndTime();
	bool addClip(AGIClipPtr clip);

	// 播放相关操作
public:
	Milliseconds getCurrentPlayTime();

private:
	std::vector<AGIClipPtr> m_clips;

private:
	Milliseconds m_curPlayTime;
};
typedef std::shared_ptr<AGITrack> AGITrackPtr;


#endif /* AGITrack_h */
