//
//  AGIFilterSetting.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterSetting.hpp"


template <typename T>
AGIFilterSetting<T>::AGIFilterSetting()
	: m_setFunc{nullptr}
	, m_defaultValue{0}
	, m_timeValueMap{}
{
	
}

template <typename T>
AGIFilterSetting<T>::~AGIFilterSetting()
{
	m_setFunc = nullptr;
	m_defaultValue = 0;
	m_timeValueMap.clear();
}

template <typename T>
bool AGIFilterSetting<T>::init(SetFunc setFunc, T defaultValue)
{
	m_setFunc = setFunc;
	m_defaultValue = defaultValue;

	return true;
}

template <typename T>
bool AGIFilterSetting<T>::setValueForTime(float time, T value)
{
	m_timeValueMap[time] = value;

	return true;
}

template <typename T>
T AGIFilterSetting<T>::getValueForTime(float getTime)
{
	auto ret = m_defaultValue;
	for (auto timeValue : m_timeValueMap)
	{
		auto time = timeValue.first;
		auto value = timeValue.second;

		if (time > getTime)
		{
			break;
		}

		ret = value;
	}

	return ret;
}

