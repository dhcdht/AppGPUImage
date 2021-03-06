//
//  AGIFilterSetting.hpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/30.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterSetting_hpp
#define AGIFilterSetting_hpp

#include <iostream>
#include <functional>
#include "AGIFilter.h"
#include <map>
#include "AGICurve.h"
#include "AGIFilterSettingInterface.h"


/*
 * AGIFilter 的设置类
 * 基本功能，在 0~1 的进度范围内给特定的 filter 设置参数值
 */
template <typename T>
class AGIFilterSetting : public AGIFilterSettingInterface
{
public:
	typedef std::function<bool(T)> SetFunc;

public:
	AGIFilterSetting();
	virtual ~AGIFilterSetting();

	bool init(SetFunc setFunc, T defaultValue = 0);

public:
	bool setValueForProgress(double progress, T value, AGICurve::Type curveType = AGICurve::None);
	T getValueForProgress(double getProgress);

    //region AGIFilterSettingInterface
public:
	bool doSetFuncForProgress(double progress) override;
    //endregion AGIFilterSettingInterface

private:
	SetFunc m_setFunc;
	T m_defaultValue;

	typedef std::pair<AGICurvePtr, T> ValueItem;
	std::map<double, ValueItem> m_progressValueMap;
};


#endif /* AGIFilterSetting_hpp */
