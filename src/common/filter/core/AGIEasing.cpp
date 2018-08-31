//
//  AGIEasing.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/31.
//  Copyright © 2018年 董宏昌. All rights reserved.
//
//	Copy from https://github.com/qt/qt/blob/0a2f2382541424726168804be2c90b91381608c6/src/3rdparty/easing/easing.cpp
//

#include <math.h>
#include <algorithm>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI / 2)
#endif

/**
 * Easing equation function for a simple linear tweening, with no easing.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeNone(double progress)
{
	return progress;
}

/**
 * Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInQuad(double t)
{
	return t * t;
}

/**
* Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
*
* @param t		Current time (in frames or seconds).
* @return		The correct value.
*/
static double easeOutQuad(double t)
{
	return -t * (t - 2);
}

/**
 * Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutQuad(double t)
{
	t *= 2.0;
	if (t < 1) {
		return t * t / double(2);
	}
	else {
		--t;
		return -0.5 * (t*(t - 2) - 1);
	}
}

/**
 * Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInQuad(double t)
{
	if (t < 0.5) return easeOutQuad(t * 2) / 2;
	return easeInQuad((2 * t) - 1) / 2 + 0.5;
}

/**
 * Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInCubic(double t)
{
	return t * t*t;
}

/**
 * Easing equation function for a cubic (t^3) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutCubic(double t)
{
	t -= 1.0;
	return t * t*t + 1;
}

/**
 * Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutCubic(double t)
{
	t *= 2.0;
	if (t < 1) {
		return 0.5*t*t*t;
	}
	else {
		t -= double(2.0);
		return 0.5*(t*t*t + 2);
	}
}

/**
 * Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInCubic(double t)
{
	if (t < 0.5) return easeOutCubic(2 * t) / 2;
	return easeInCubic(2 * t - 1) / 2 + 0.5;
}

/**
 * Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInQuart(double t)
{
	return t * t*t*t;
}

/**
 * Easing equation function for a quartic (t^4) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutQuart(double t)
{
	t -= double(1.0);
	return -(t*t*t*t - 1);
}

/**
 * Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutQuart(double t)
{
	t *= 2;
	if (t < 1) return 0.5*t*t*t*t;
	else {
		t -= 2.0f;
		return -0.5 * (t*t*t*t - 2);
	}
}

/**
 * Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInQuart(double t)
{
	if (t < 0.5) return easeOutQuart(2 * t) / 2;
	return easeInQuart(2 * t - 1) / 2 + 0.5;
}

/**
 * Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInQuint(double t)
{
	return t * t*t*t*t;
}

/**
 * Easing equation function for a quintic (t^5) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutQuint(double t)
{
	t -= 1.0;
	return t * t*t*t*t + 1;
}

/**
 * Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutQuint(double t)
{
	t *= 2.0;
	if (t < 1) return 0.5*t*t*t*t*t;
	else {
		t -= 2.0;
		return 0.5*(t*t*t*t*t + 2);
	}
}

/**
 * Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInQuint(double t)
{
	if (t < 0.5) return easeOutQuint(2 * t) / 2;
	return easeInQuint(2 * t - 1) / 2 + 0.5;
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInSine(double t)
{
	return (t == 1.0) ? 1.0 : -::cos(t * M_PI_2) + 1.0;
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutSine(double t)
{
	return ::sin(t* M_PI_2);
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutSine(double t)
{
	return -0.5 * (::cos(M_PI*t) - 1);
}

/**
 * Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInSine(double t)
{
	if (t < 0.5) return easeOutSine(2 * t) / 2;
	return easeInSine(2 * t - 1) / 2 + 0.5;
}

/**
 * Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInExpo(double t)
{
	return (t == 0 || t == 1.0) ? t : ::pow(2.0, 10 * (t - 1)) - double(0.001);
}

/**
 * Easing equation function for an exponential (2^t) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutExpo(double t)
{
	return (t == 1.0) ? 1.0 : 1.001 * (-::pow(2.0f, -10 * t) + 1);
}

/**
 * Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutExpo(double t)
{
	if (t == 0.0) return double(0.0);
	if (t == 1.0) return double(1.0);
	t *= 2.0;
	if (t < 1) return 0.5 * ::pow(double(2.0), 10 * (t - 1)) - 0.0005;
	return 0.5 * 1.0005 * (-::pow(double(2.0), -10 * (t - 1)) + 2);
}

/**
 * Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInExpo(double t)
{
	if (t < 0.5) return easeOutExpo(2 * t) / 2;
	return easeInExpo(2 * t - 1) / 2 + 0.5;
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInCirc(double t)
{
	return -(::sqrt(1 - t * t) - 1);
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutCirc(double t)
{
	t -= double(1.0);
	return ::sqrt(1 - t * t);
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeInOutCirc(double t)
{
	t *= double(2.0);
	if (t < 1) {
		return -0.5 * (::sqrt(1 - t * t) - 1);
	}
	else {
		t -= double(2.0);
		return 0.5 * (::sqrt(1 - t * t) + 1);
	}
}

/**
 * Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @return		The correct value.
 */
static double easeOutInCirc(double t)
{
	if (t < 0.5) return easeOutCirc(2 * t) / 2;
	return easeInCirc(2 * t - 1) / 2 + 0.5;
}

static double easeInElastic_helper(double t, double b, double c, double d, double a, double p)
{
	if (t == 0) return b;
	double t_adj = (double)t / (double)d;
	if (t_adj == 1) return b + c;

	double s;
	if (a < ::fabs(c)) {
		a = c;
		s = p / 4.0f;
	}
	else {
		s = p / (2 * M_PI) * ::asin(c / a);
	}

	t_adj -= 1.0f;
	return -(a*::pow(2.0f, 10 * t_adj) * ::sin((t_adj*d - s)*(2 * M_PI) / p)) + b;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static double easeInElastic(double t, double a, double p)
{
	return easeInElastic_helper(t, 0, 1, 1, a, p);
}

static double easeOutElastic_helper(double t, double /*b*/, double c, double /*d*/, double a, double p)
{
	if (t == 0) return 0;
	if (t == 1) return c;

	double s;
	if (a < c) {
		a = c;
		s = p / 4.0f;
	}
	else {
		s = p / (2 * M_PI) * ::asin(c / a);
	}

	return (a*::pow(2.0f, -10 * t) * ::sin((t - s)*(2 * M_PI) / p) + c);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static double easeOutElastic(double t, double a, double p)
{
	return easeOutElastic_helper(t, 0, 1, 1, a, p);
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static double easeInOutElastic(double t, double a, double p)
{
	if (t == 0) return 0.0;
	t *= 2.0;
	if (t == 2) return 1.0;

	double s;
	if (a < 1.0) {
		a = 1.0;
		s = p / 4.0f;
	}
	else {
		s = p / (2 * M_PI) * ::asin(1.0 / a);
	}

	if (t < 1) return -.5*(a*::pow(2.0f, 10 * (t - 1)) * ::sin((t - 1 - s)*(2 * M_PI) / p));
	return a * ::pow(2.0f, -10 * (t - 1)) * ::sin((t - 1 - s)*(2 * M_PI) / p)*.5 + 1.0;
}

/**
 * Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @param p		Period.
 * @return		The correct value.
 */
static double easeOutInElastic(double t, double a, double p)
{
	if (t < 0.5) return easeOutElastic_helper(t * 2, 0, 0.5, 1.0, a, p);
	return easeInElastic_helper(2 * t - 1.0, 0.5, 0.5, 1.0, a, p);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static double easeInBack(double t, double s)
{
	return t * t*((s + 1)*t - s);
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static double easeOutBack(double t, double s)
{
	t -= double(1.0);
	return t * t*((s + 1)*t + s) + 1;
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static double easeInOutBack(double t, double s)
{
	t *= 2.0;
	if (t < 1) {
		s *= 1.525f;
		return 0.5*(t*t*((s + 1)*t - s));
	}
	else {
		t -= 2;
		s *= 1.525f;
		return 0.5*(t*t*((s + 1)*t + s) + 2);
	}
}

/**
 * Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param s		Overshoot ammount: higher s means greater overshoot (0 produces cubic easing with no overshoot, and the default value of 1.70158 produces an overshoot of 10 percent).
 * @return		The correct value.
 */
static double easeOutInBack(double t, double s)
{
	if (t < 0.5) return easeOutBack(2 * t, s) / 2;
	return easeInBack(2 * t - 1, s) / 2 + 0.5;
}

static double easeOutBounce_helper(double t, double c, double a)
{
	if (t == 1.0) return c;
	if (t < (4 / 11.0)) {
		return c * (7.5625*t*t);
	}
	else if (t < (8 / 11.0)) {
		t -= (6 / 11.0);
		return -a * (1. - (7.5625*t*t + .75)) + c;
	}
	else if (t < (10 / 11.0)) {
		t -= (9 / 11.0);
		return -a * (1. - (7.5625*t*t + .9375)) + c;
	}
	else {
		t -= (21 / 22.0);
		return -a * (1. - (7.5625*t*t + .984375)) + c;
	}
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static double easeOutBounce(double t, double a)
{
	return easeOutBounce_helper(t, 1, a);
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static double easeInBounce(double t, double a)
{
	return 1.0 - easeOutBounce_helper(1.0 - t, 1.0, a);
}


/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static double easeInOutBounce(double t, double a)
{
	if (t < 0.5) return easeInBounce(2 * t, a) / 2;
	else return (t == 1.0) ? 1.0 : easeOutBounce(2 * t - 1, a) / 2 + 0.5;
}

/**
 * Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
 *
 * @param t		Current time (in frames or seconds).
 * @param a		Amplitude.
 * @return		The correct value.
 */
static double easeOutInBounce(double t, double a)
{
	if (t < 0.5) return easeOutBounce_helper(t * 2, 0.5, a);
	return 1.0 - easeOutBounce_helper(2.0 - 2 * t, 0.5, a);
}

static inline double qt_sinProgress(double value)
{
	return sin((value * M_PI) - M_PI_2) / 2 + double(0.5);
}

static inline double qt_smoothBeginEndMixFactor(double value)
{
	return std::min(std::max(1 - value * 2 + double(0.3), double(0.0)), double(1.0));
}

// SmoothBegin blends Smooth and Linear Interpolation.
// Progress 0 - 0.3      : Smooth only
// Progress 0.3 - ~ 0.5  : Mix of Smooth and Linear
// Progress ~ 0.5  - 1   : Linear only

/**
 * Easing function that starts growing slowly, then increases in speed. At the end of the curve the speed will be constant.
 */
static double easeInCurve(double t)
{
	const double sinProgress = qt_sinProgress(t);
	const double mix = qt_smoothBeginEndMixFactor(t);
	return sinProgress * mix + t * (1 - mix);
}

/**
 * Easing function that starts growing steadily, then ends slowly. The speed will be constant at the beginning of the curve.
 */
static double easeOutCurve(double t)
{
	const double sinProgress = qt_sinProgress(t);
	const double mix = qt_smoothBeginEndMixFactor(1 - t);
	return sinProgress * mix + t * (1 - mix);
}

/**
 * Easing function where the value grows sinusoidally. Note that the calculated  end value will be 0 rather than 1.
 */
static double easeSineCurve(double t)
{
	return (sin(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}

/**
 * Easing function where the value grows cosinusoidally. Note that the calculated start value will be 0.5 and the end value will be 0.5
 * contrary to the usual 0 to 1 easing curve.
 */
static double easeCosineCurve(double t)
{
	return (cos(((t * M_PI * 2)) - M_PI_2) + 1) / 2;
}
