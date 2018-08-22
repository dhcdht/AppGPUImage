//
//  AGIImage.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/30.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIImage_h
#define AGIImage_h

#include <iostream>
#include "bgfx/bgfx.h"


class AGIImage
{
public:
	AGIImage(bgfx::TextureHandle textureHandle, int sizeWidth, int sizeHeight, bgfx::TextureFormat::Enum format);
	AGIImage(bgfx::FrameBufferHandle framebufferHandle, int sizeWidth, int sizeHeight, bgfx::TextureFormat::Enum format);
	virtual ~AGIImage();

public:
	bgfx::Memory* getCPUBuffer();
    bgfx::Memory* getCPUBufferFocre();
	bgfx::TextureHandle getGPUTextureHandle();

	int getSizeWidth();
	int getSizeHeight();
	bgfx::TextureFormat::Enum getFormat();

private:
	bgfx::Memory *m_CPUBuffer;
	bgfx::TextureHandle m_GPUTextureHandle;
	bgfx::FrameBufferHandle m_GPUFramebufferHandle;

	int m_sizeWidth;
	int m_sizeHeight;
	bgfx::TextureFormat::Enum m_format;
};
typedef std::shared_ptr<AGIImage> AGIImagePtr;


#endif /* AGIImage_h */
