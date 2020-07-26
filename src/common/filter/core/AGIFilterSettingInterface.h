//
//  AGIFilterSettingInterface.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/11.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterSettingInterface_h
#define AGIFilterSettingInterface_h

#include <iostream>
#include <unordered_set>


class AGIFilterSettingInterface
{
public:
    virtual bool doSetFuncForProgress(double progress) = 0;
};
typedef std::shared_ptr<AGIFilterSettingInterface> AGIFilterSettingInterfacePtr;


class AGIFilterSettingContainer : public AGIFilterSettingInterface
{
public:
    AGIFilterSettingContainer();
    virtual ~AGIFilterSettingContainer();

public:
    bool addFilterSetting(AGIFilterSettingInterfacePtr setting);
    bool isContainFilterSetting(AGIFilterSettingInterfacePtr setting);
    bool removeFilterSetting(AGIFilterSettingInterfacePtr setting);

    //region AGIFilterSettingInterface
public:
    bool doSetFuncForProgress(double progress) override;
    //endregion AGIFilterSettingInterface

private:
    std::unordered_set<AGIFilterSettingInterfacePtr> m_settings;
    std::recursive_mutex m_settingsMutex;
};
typedef std::shared_ptr<AGIFilterSettingContainer> AGIFilterSettingContainerPtr;


#endif /* AGIFilterSettingInterface_h */
