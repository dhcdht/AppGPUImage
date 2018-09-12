//
//  AGIImageConnection.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/12.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIImageConnection.h"


AGIImageConnection::AGIImageConnection(AGIPiplineSourcePtr source, AGIPiplineTargetPtr target)
    : AGIPiplineConnection<AGIImagePtr, AGIImagePtr>(source, target)
{

}

AGIImageConnection::~AGIImageConnection()
{
    
}

