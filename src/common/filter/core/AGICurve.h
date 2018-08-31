//
//  AGICurve.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/31.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGICurve_h
#define AGICurve_h

#include <iostream>


/*
 * 曲线类
 * 将 0~1 的值通过特定的曲线函数做转换
 * 注意：有些曲线输出值范围不是 0~1
 * 在 http://doc.qt.io/qt-5/qeasingcurve.html 中可以看到各个曲线的图形
 */
class AGICurve
{
public:
	enum Type
	{
		None = 0,
		Linear,
		InQuad, OutQuad, InOutQuad, OutInQuad,
		InCubic, OutCubic, InOutCubic, OutInCubic,
		InQuart, OutQuart, InOutQuart, OutInQuart,
		InQuint, OutQuint, InOutQuint, OutInQuint,
		InSine, OutSine, InOutSine, OutInSine,
		InExpo, OutExpo, InOutExpo, OutInExpo,
		InCirc, OutCirc, InOutCirc, OutInCirc,
		//InElastic, OutElastic, InOutElastic, OutInElastic,
		//InBack, OutBack, InOutBack, OutInBack,
		//InBounce, OutBounce, InOutBounce, OutInBounce,
		//InCurve, OutCurve, SineCurve, CosineCurve,
		Custom,
	};

public:
	AGICurve();
	virtual ~AGICurve();

	bool init(Type type = None);

public:
	double getCurveValueForPoint(double point);

private:
	Type m_type;
};
typedef std::shared_ptr<AGICurve> AGICurvePtr;


#endif /* AGICurve_h */
