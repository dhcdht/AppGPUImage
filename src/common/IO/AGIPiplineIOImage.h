//
//  AGIPiplineIOImage.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineIOImage_h
#define AGIPiplineIOImage_h

#include "input/AGIPiplineInput.h"
#include "output/AGIPiplineOutput.h"


class AGIPiplineIOImageInputDelegate : public AGIPiplineInput
{

};
typedef std::shared_ptr<AGIPiplineIOImageInputDelegate> AGIPiplineIOImageInputDelegatePtr;


class AGIPiplineIOImage : public AGIPiplineInput, public AGIPiplineOutput
{
public:
	AGIPiplineIOImage();
	~AGIPiplineIOImage() override;

	bool init(const std::string filePath) override { return false; };
	bool init() override;

public:
	void setInputParamsDelegate(AGIPiplineIOImageInputDelegatePtr inputParamsDelegate);
	AGIPiplineIOImageInputDelegatePtr getInputParamsDelegate();

	//region AGIPiplineInput

	//region AGIPiplineSource
public:
	int getSourceOutputCount() override { return 1; };
	std::vector<AGIImagePtr> pullOutputs() override;
	//endregion AGIPiplineSource

	Milliseconds getDuration() override;
	int getPreferFrameRate() override;
	bool syncSeekToTime(Milliseconds time) override;
	Milliseconds getCurrentFrameTime() override;
	Milliseconds getCurrentFrameDuration() override;

	//endregion AGIPiplineInput

	//region AGIPiplineTarget
public:
	int getTargetInputCount() override { return 1; };
	bool processTarget() override;
	void endOneProcess() override;
	//endregion AGIPiplineTarget

public:
	AGIImagePtr getLastOutputImage();

private:
	AGIImagePtr m_lastOutputImage;

	AGIPiplineIOImageInputDelegatePtr m_inputParamsDelegate;
};
typedef std::shared_ptr<AGIPiplineIOImage> AGIPiplineIOImagePtr;


#endif /* AGIPiplineIOImage_h */
