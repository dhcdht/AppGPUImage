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
	, m_progressValueMap{}
{
	
}

template <typename T>
AGIFilterSetting<T>::~AGIFilterSetting()
{
	m_setFunc = nullptr;
	m_defaultValue = 0;
	m_progressValueMap.clear();
}

template <typename T>
bool AGIFilterSetting<T>::init(SetFunc setFunc, T defaultValue)
{
	m_setFunc = setFunc;
	m_defaultValue = defaultValue;

	return true;
}

template <typename T>
bool AGIFilterSetting<T>::setValueForProgress(double progress, T value, AGICurve::Type curveType)
{
	auto curve = std::make_shared<AGICurve>();
	curve->init(curveType);
	m_progressValueMap[progress] = {curve, value};

	return true;
}

template <typename T>
T AGIFilterSetting<T>::getValueForProgress(double getProgress)
{
	float leftProgress = 0.0;
	float leftValue = m_defaultValue;
	float ret = leftValue;
	for (auto item : m_progressValueMap)
	{
		ValueItem progressValue = item.second;
		float progress = item.first;
		float value = progressValue.second;

		if (progress > getProgress)
		{
			auto rightProgress = progress;
			auto rightValue = value;
			AGICurvePtr curve = progressValue.first;
			auto curPoint = (getProgress - leftProgress) / (rightProgress - leftProgress);
			auto curveValue = curve->getCurveValueForPoint(curPoint);

			ret = leftValue + (rightValue - leftValue) * curveValue;

			break;
		}

		leftProgress = progress;
		leftValue = value;
	}

	return ret;
}

template <typename T>
bool AGIFilterSetting<T>::doSetFuncForProgress(double progress)
{
	auto value = getValueForProgress(progress);
	return m_setFunc(value);
}

