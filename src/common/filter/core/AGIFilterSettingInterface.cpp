//
//  AGIFilterSettingInterface.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/11.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterSettingInterface.h"


AGIFilterSettingContainer::AGIFilterSettingContainer()
    : m_settings{}
    , m_settingsMutex{}
{

}

AGIFilterSettingContainer::~AGIFilterSettingContainer()
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_settingsMutex);

    m_settings.clear();
}

bool AGIFilterSettingContainer::addFilterSetting(AGIFilterSettingInterfacePtr setting)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_settingsMutex);

    m_settings.insert(setting);

    return true;
}

bool AGIFilterSettingContainer::isContainFilterSetting(AGIFilterSettingInterfacePtr setting)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_settingsMutex);

    if (m_settings.find(setting) != m_settings.end()) {
        return true;
    } else {
        return false;
    }
}

bool AGIFilterSettingContainer::removeFilterSetting(AGIFilterSettingInterfacePtr setting)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_settingsMutex);

    m_settings.erase(setting);

    return true;
}

bool AGIFilterSettingContainer::doSetFuncForProgress(double progress)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_settingsMutex);

    bool ret = true;
    for (auto setting : m_settings) {
        ret &= setting->doSetFuncForProgress(progress);
    }

    return ret;
}

