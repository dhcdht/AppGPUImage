//
//  AGIResourceManager.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIResourceManager.h"


AGIResourceManager::~AGIResourceManager() {

}

AGIResourceManager::AGIResourceManager() {

}

AGIResourceManager* AGIResourceManager::sharedBundle() {
    static auto kSharedBundle = new AGIResourceManager();

    return kSharedBundle;
}

void AGIResourceManager::setBundlePath(std::string bundlePath) {
	while (bundlePath.find("\\") != std::string::npos)
	{
		bundlePath.replace(bundlePath.find("\\"), 1, "/");
	}
    if (bundlePath[bundlePath.length()-1] != '/') {
        m_bundlePath = bundlePath + "/";
    } else {
        m_bundlePath = bundlePath;
    }
}

std::string AGIResourceManager::getBundlePath() {
    return m_bundlePath;
}

std::string AGIResourceManager::getBundleResourcePath(std::string fileName) {
    std::string filePath = m_bundlePath + fileName;

    return filePath;
}
