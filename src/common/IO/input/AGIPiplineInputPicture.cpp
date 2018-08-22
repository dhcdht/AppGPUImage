//
//  AGIPiplineInputPicture.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/3.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineInputPicture.h"
#include "util/bgfxUtils.h"
#include "util/AGIUtilDefer.h"


AGIPiplineInputPicture::AGIPiplineInputPicture()
: m_image(nullptr)
{

}

AGIPiplineInputPicture::~AGIPiplineInputPicture()
{
	m_image = nullptr;
}

bool AGIPiplineInputPicture::init(bgfx::TextureHandle textureHandle)
{
    if (!bgfx::isValid(textureHandle)) {
        return false;
    }

	m_image = std::make_shared<AGIImage>(
		textureHandle, 0, 0, bgfx::TextureFormat::Enum::RGBA8
		);

    return true;
}

bgfx::TextureHandle AGIPiplineInputPicture::getTextureHandle()
{
    return m_image->getGPUTextureHandle();
}

bool AGIPiplineInputPicture::init(const std::string filePath)
{
	auto ic = bgfxUtils::loadImage(filePath.c_str());
	CleanDefer([&]()
	{
		bimg::imageFree(ic);
	});
    auto textureHandle = bgfxUtils::loadTexture(filePath.c_str());
    if (!bgfx::isValid(textureHandle)) {
        return false;
    }

	m_image = std::make_shared<AGIImage>(
		textureHandle, ic->m_width, ic->m_height, (bgfx::TextureFormat::Enum)ic->m_format
		);

    return true;
}

std::chrono::milliseconds AGIPiplineInputPicture::getDuration()
{
	return std::chrono::milliseconds(1);
}


int AGIPiplineInputPicture::getSourceOutputCount()
{
    return 1;
}

std::vector<AGIImagePtr> AGIPiplineInputPicture::pullOutputs()
{
	return { m_image };
}

