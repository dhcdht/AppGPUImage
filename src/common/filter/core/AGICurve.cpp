//
//  AGICurve.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/31.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGICurve.h"
#include "AGIEasing.cpp"


AGICurve::AGICurve()
	: m_type{None}
{
	
}

AGICurve::~AGICurve()
{
	m_type = None;
}

bool AGICurve::init(Type type)
{
	m_type = type;

	return true;
}

double AGICurve::getCurveValueForPoint(double point)
{
	if (m_type == None)
	{
		return 0.0;
	}
	else if (m_type == Custom)
	{
		// todo: Custom Curve
		return 0.0;
	}

	switch (m_type)
	{
		case Linear: {
			return easeNone(point);
		} break;

		case InQuad: {
			return easeInQuad(point);
		} break;
		case OutQuad: {
			return easeOutQuad(point);
		} break;
		case InOutQuad: {
			return easeInOutQuad(point);
		} break;
		case OutInQuad: {
			return easeOutInQuad(point);
		} break;

		case InCubic: {
			return easeInCubic(point);
		} break;
		case OutCubic: {
			return easeOutCubic(point);
		} break;
		case InOutCubic: {
			return easeInOutCubic(point);
		} break;
		case OutInCubic: {
			return easeOutInCubic(point);
		} break;

		case InQuart: {
			return easeInQuart(point);
		} break;
		case OutQuart: {
			return easeOutQuart(point);
		} break;
		case InOutQuart: {
			return easeInOutQuart(point);
		} break;
		case OutInQuart: {
			return easeOutInQuart(point);
		} break;

		case InQuint: {
			return easeInQuint(point);
		} break;
		case OutQuint: {
			return easeOutQuint(point);
		} break;
		case InOutQuint: {
			return easeInOutQuint(point);
		} break;
		case OutInQuint: {
			return easeOutInQuint(point);
		} break;

		case InSine: {
			return easeInSine(point);
		} break;
		case OutSine: {
			return easeOutSine(point);
		} break;
		case InOutSine: {
			return easeInOutSine(point);
		} break;
		case OutInSine: {
			return easeOutInSine(point);
		} break;

		case InExpo: {
			return easeInExpo(point);
		} break;
		case OutExpo: {
			return easeOutExpo(point);
		} break;
		case InOutExpo: {
			return easeInOutExpo(point);
		} break;
		case OutInExpo: {
			return easeOutInExpo(point);
		} break;

		case InCirc: {
			return easeInCirc(point);
		} break;
		case OutCirc: {
			return easeOutCirc(point);
		} break;
		case InOutCirc: {
			return easeInOutCirc(point);
		} break;
		case OutInCirc: {
			return easeOutInCirc(point);
		} break;

		default: {
			return 0.0;
		} break;
	}

	return 0.0;
}

