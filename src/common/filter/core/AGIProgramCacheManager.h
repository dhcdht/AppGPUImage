//
//  AGIProgramCacheManager.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIProgramCacheManager_h
#define AGIProgramCacheManager_h

#include <iostream>
#include <unordered_map>
#include "AGIProgram.h"


class AGIProgramCacheManager {
public:
    virtual ~AGIProgramCacheManager();
    AGIProgramCacheManager();

    static AGIProgramCacheManager* sharedManager();

public:
    virtual bool setProgram(std::string key, std::shared_ptr<AGIProgram> program);
    virtual std::shared_ptr<AGIProgram> getProgram(std::string key);

protected:
    std::unordered_map<std::string, std::shared_ptr<AGIProgram>> m_programMap;
};


#endif /* AGIProgramCacheManager_h */
