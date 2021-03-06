//
//  AGIImageConnection.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/12.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIImageConnection_h
#define AGIImageConnection_h

#include "pipline/AGIPiplineConnection.hpp"
#include "AGIImage.h"


class AGIImageConnection : public AGIPiplineConnection<AGIImagePtr, AGIImagePtr>
{
public:
    typedef std::shared_ptr<AGIPiplineSource<AGIImagePtr>> AGIPiplineSourcePtr;
    typedef std::shared_ptr<AGIPiplineTarget<AGIImagePtr>> AGIPiplineTargetPtr;

public:
    AGIImageConnection(AGIPiplineSourcePtr source, AGIPiplineTargetPtr target);
    ~AGIImageConnection() override;
};


#endif /* AGIImageConnection_h */
