//
//  AGIUtilDefer.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIUtilDefer_h
#define AGIUtilDefer_h

#include <functional>


typedef std::function<void(void)> AGIUtilDeferFunc;


class AGIUtilDefer {
public:
    AGIUtilDefer(const AGIUtilDeferFunc &func);
    virtual ~AGIUtilDefer();

    void operator=(const std::function<void(void)> func);

public:
    void cancel();

private:
    AGIUtilDeferFunc m_func;
};
typedef AGIUtilDefer GuardDefer;


// 做两次转换时为了确保 s1 s2 一定会被解析，比如 __LINE__ 解析为行号
#define __SCOPEGUARD_CONCATENATE_IMPL(s1, s2) s1##s2
#define __SCOPEGUARD_CONCATENATE(s1, s2) __SCOPEGUARD_CONCATENATE_IMPL(s1, s2)
#define CleanDefer(func) GuardDefer __SCOPEGUARD_CONCATENATE(__deferCleaner, __LINE__) = {func}


#endif /* AGIUtilDefer_h */
