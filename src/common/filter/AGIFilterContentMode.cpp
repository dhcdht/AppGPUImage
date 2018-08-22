//
//  AGIFilterContentMode.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterContentMode.h"
#include "bx/math.h"
#include "core/AGIContext.h"


AGIFilterContentMode::AGIFilterContentMode()
	: m_contentMode(ResizeFull)
{
	
}

AGIFilterContentMode::~AGIFilterContentMode()
{
	
}

bool AGIFilterContentMode::setContentMode(ContentMode contentMode)
{
	m_contentMode = contentMode;

	return true;
}

bool AGIFilterContentMode::commitUniform()
{
	float transform[4][4];
	bx::mtxIdentity((float*)transform);
	if (m_textures.size() > 0)
	{
		switch (m_contentMode)
		{
		case AspectFit:
			{
				double sizeWidth = m_textures[0]->getSizeWidth();
				double sizeHeight = m_textures[0]->getSizeHeight();
				double windowWidth = AGIContext::sharedContext()->getWindowSizeWidth();
				double windowHeight = AGIContext::sharedContext()->getWindowSizeHeight();
				double sizeRatio = sizeWidth / sizeHeight;
				double windowRatio = windowWidth / windowHeight;
				if (sizeRatio >= windowRatio)
				{
					bx::mtxScale((float*)transform, 1.0, (sizeHeight * (windowWidth / sizeWidth)) / windowHeight, 1.0);
				}
				else
				{
					bx::mtxScale((float*)transform, (sizeWidth * (windowHeight / sizeHeight)) / windowWidth, 1.0, 1.0);
				}

			break;
			}

		case AspectFill:
			{
				double sizeWidth = m_textures[0]->getSizeWidth();
				double sizeHeight = m_textures[0]->getSizeHeight();
				double windowWidth = AGIContext::sharedContext()->getWindowSizeWidth();
				double windowHeight = AGIContext::sharedContext()->getWindowSizeHeight();
				double sizeRatio = sizeWidth / sizeHeight;
				double windowRatio = windowWidth / windowHeight;
				if (sizeRatio < windowRatio)
				{
					bx::mtxScale((float*)transform, 1.0, (sizeHeight * (windowWidth / sizeWidth)) / windowHeight, 1.0);
				}
				else
				{
					bx::mtxScale((float*)transform, (sizeWidth * (windowHeight / sizeHeight)) / windowWidth, 1.0, 1.0);
				}

			break;
			}

		case ResizeFull:
		default:
			{
				bx::mtxIdentity((float*)transform);

			break;
			}
		}
	}
	this->setTransform(transform);

	if (!AGIFilterTransform::commitUniform())
	{
		return false;
	}

	return true;
}


