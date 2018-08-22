//
//  AGIFilterContentMode.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterContentMode_h
#define AGIFilterContentMode_h

#include "AGIFilterTransform.h"


class AGIFilterContentMode : public AGIFilterTransform
{
public:
	AGIFilterContentMode();
	~AGIFilterContentMode() override;

public:
	enum ContentMode
	{
		ResizeFull = 0,
		AspectFit,
		AspectFill,
	};
	bool setContentMode(ContentMode contentMode);

protected:
	bool commitUniform() override;

private:
	ContentMode m_contentMode;
};


#endif /* AGIFilterContentMode_h */
