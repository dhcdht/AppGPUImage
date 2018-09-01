//
//  AGIFilterSetting.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/30.
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
bool AGIFilterSetting<T>::setValueForTime(double time, T value, AGICurve::Type curveType)
{
	auto curve = std::make_shared<AGICurve>();
	curve->init(curveType);
	m_timeValueMap[time] = {curve, value};

	return true;
}

template <typename T>
T AGIFilterSetting<T>::getValueForTime(double getTime)
{
	float leftTime = 0.0;
	float leftValue = m_defaultValue;
	float ret = leftValue;
	for (auto item : m_timeValueMap)
	{
		ValueItem timeValue = item.second;
		float time = item.first;
		float value = timeValue.second;

		if (time > getTime)
		{
			auto rightTime = time;
			auto rightValue = value;
			AGICurvePtr curve = timeValue.first;
			auto curPoint = (getTime - leftTime) / (rightTime - leftTime);
			auto curveValue = curve->getCurveValueForPoint(curPoint);

			ret = leftValue + (rightValue - leftValue) * curveValue;

			break;
		}

		leftTime = time;
		leftValue = value;
	}

	return ret;
}

template <typename T>
bool AGIFilterSetting<T>::doSetFuncForTime(double getTime)
{
	auto value = getValueForTime(getTime);
	return m_setFunc(value);
}

