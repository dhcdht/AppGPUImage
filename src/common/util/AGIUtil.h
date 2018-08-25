//
//  AGIUtil.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIUtil_h
#define AGIUtil_h

#include "bgfxUtils.h"
#include "AGIUtilDefer.h"


#ifdef _DEBUG
#define AGIDEBUG _DEBUG
#endif
#ifdef DEBUG
#define AGIDEBUG _DEBUG
#endif


#ifdef AGIDEBUG
#define AGIAssert(x, ...) assert(x, __VA_ARGS__)
#else
#define AGIAssert(x, ...) 
#endif


#endif /* AGIUtil_h */
