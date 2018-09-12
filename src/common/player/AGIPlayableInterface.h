//
//  AGIPlayableInterface.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/12.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPlayableInterface_h
#define AGIPlayableInterface_h

#include "IO/input/AGIPiplineInput.h"
#include "IO/AGIPiplineIOImage.h"
#include "filter/core/AGIFilterGraph.h"


class AGIPlayableInterface
{
public:
    virtual AGIPiplineInputPtr getSourceInputPtr() = 0;
    virtual AGIPiplineIOImagePtr getTargetIOImagePtr() = 0;

    virtual AGIFilterGraphPtr getFilterGraphForTime(Milliseconds time) = 0;
};
typedef std::shared_ptr<AGIPlayableInterface> AGIPlayableInterfacePtr;


#endif /* AGIPlayableInterface_h */
