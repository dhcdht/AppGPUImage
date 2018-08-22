//
//  bgfxUtils.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef bgfxUtils_hpp
#define bgfxUtils_hpp

#include <iostream>
#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <chrono>
#include "opencv/cv.hpp"


// 方便书写着色器的宏
#define SHADER_STRING(text) #text


// 毫秒时间的定义
typedef std::chrono::milliseconds Milliseconds;


namespace bgfxUtils {
    bimg::ImageContainer *loadImage(const char *filePath);
    bgfx::TextureHandle loadTexture(const char *filePath);
    bgfx::TextureHandle loadTexture(const void* data, uint32_t size);

    enum ShaderType {
        vertex = 'v',
        fragment = 'f',
        compute = 'c',
    };
    bgfx::ShaderHandle createShader(ShaderType type,
                                    const char *filePath,
                                    const char *defines = nullptr,
                                    const char *varyingPath = nullptr,
                                    const char *profile = nullptr);

	cv::Mat generateCVMatFromBuffer(bgfx::Memory *mem, bgfx::TextureFormat::Enum format, int width, int height);
	bgfx::TextureHandle generateTextureFromBuffer(bgfx::Memory *mem, bgfx::TextureFormat::Enum format, int width, int height);
}


#endif /* bgfxUtils_hpp */
