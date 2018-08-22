//
//  AGIUtilDefer.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIUtilDefer.h"


AGIUtilDefer::AGIUtilDefer(const AGIUtilDeferFunc &func)
    : m_func(std::move(func))
{

}

AGIUtilDefer::~AGIUtilDefer()
{
    if (m_func != nullptr) {
        m_func();
        m_func = nullptr;
    }
}

void AGIUtilDefer::cancel()
{
    m_func = nullptr;
}

