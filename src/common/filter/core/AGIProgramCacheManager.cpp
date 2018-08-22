//
//  AGIProgramCacheManager.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIProgramCacheManager.h"


AGIProgramCacheManager::~AGIProgramCacheManager() {

}

AGIProgramCacheManager::AGIProgramCacheManager()
: m_programMap(){

}

AGIProgramCacheManager* AGIProgramCacheManager::sharedManager() {
    static auto kSharedManger = new AGIProgramCacheManager();

    return kSharedManger;
}

bool AGIProgramCacheManager::setProgram(std::string key, std::shared_ptr<AGIProgram> program) {
    m_programMap[key] = program;

    return true;
}

std::shared_ptr<AGIProgram> AGIProgramCacheManager::getProgram(std::string key) {
    auto program = m_programMap[key];

    return program;
}

