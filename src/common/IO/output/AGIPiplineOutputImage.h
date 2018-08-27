//
//  AGIPiplineOutputImage.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineOutputImage_h
#define AGIPiplineOutputImage_h

#include "AGIPiplineOutput.h"


class AGIPiplineOutputImage : public AGIPiplineOutput
{
public:
	AGIPiplineOutputImage();
	~AGIPiplineOutputImage() override;

	bool init() override;

	//region AGIPiplineTarget
public:
	int getTargetInputCount() override
	{
		return 1;
	}
	bool processTarget() override;
	void endOneProcess() override;
	//endregion AGIPiplineTarget

public:
	AGIImagePtr getLastOutputImage();

private:
	AGIImagePtr m_lastOutputImage;
};


#endif /* AGIPiplineOutputImage_h */
