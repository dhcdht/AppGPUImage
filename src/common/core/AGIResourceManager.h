//
//  AGIResourceManager.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIResourceManager_hpp
#define AGIResourceManager_hpp


#include <iostream>
#include <string>


class AGIResourceManager {
public:
    virtual ~AGIResourceManager();
    AGIResourceManager();

    static AGIResourceManager* sharedBundle();

public:
    void setBundlePath(std::string bundlePath);
    std::string getBundlePath();

    std::string getBundleResourcePath(std::string fileName);

private:
    std::string m_bundlePath;
};


#endif /* AGIResourceManager_hpp */
