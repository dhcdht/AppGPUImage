//
//  AGIPiplineOutput.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/25.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineOutput_h
#define AGIPiplineOutput_h

#include "piplineIO/AGIPiplineIO.hpp"
#include "IO/AGIImage.h"


class AGIPiplineOutput : public AGIPiplineTarget<AGIImagePtr>
{
public:
    AGIPiplineOutput();
    virtual ~AGIPiplineOutput();

    virtual bool init() = 0;
};
typedef std::shared_ptr<AGIPiplineOutput> AGIPiplineOutputPtr;


#endif /* AGIPiplineOutput_h */
