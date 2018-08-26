//
//  SDKTest.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef SDKTest_h
#define SDKTest_h

#include <iostream>

class SDKTest {
public:
    virtual ~SDKTest();
    SDKTest();

    void init(void *window, int sizeWidth, int sizeHeight);

    void test_timeline(const char *filePath);

    void test_playerEngine(const char *filePath);

public:
    int m_windowID;
    int m_sizeWidth;
    int m_sizeHeight;
};

#endif /* SDKTest_h */
