//
//  AGIFilterSetting.hpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterSetting_hpp
#define AGIFilterSetting_hpp

#include <iostream>
#include <functional>
#include "AGIFilter.h"
#include <map>


/*
 * AGIFilter 的设置类
 * 基本功能，在 0~1 的时间范围内给特定的 filter 设置参数值
 */
template <typename T>
class AGIFilterSetting
{
public:
	typedef std::function<bool(T)> SetFunc;

public:
	AGIFilterSetting();
	virtual ~AGIFilterSetting();

	bool init(SetFunc setFunc, T defaultValue = 0);

public:
	bool setValueForTime(float time, T value);
	T getValueForTime(float getTime);

private:
	SetFunc m_setFunc;
	T m_defaultValue;
	std::map<float, T> m_timeValueMap;
};


#endif /* AGIFilterSetting_hpp */
