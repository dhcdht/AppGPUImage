//
//  AGIPlayable.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/12.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPlayable_h
#define AGIPlayable_h

#include "AGIPlayableInterface.h"
#include "filter/core/AGIFilterDynamicGraph.h"


class AGIPlayable : public AGIPlayableInterface
{
public:
    AGIPlayable(AGIPiplineInputPtr input, AGIPiplineIOImagePtr ioImage, AGIFilterGraphPtr filterGraph);
    AGIPlayable(AGIPiplineInputPtr input, AGIPiplineIOImagePtr ioImage, AGIFilterDynamicGraphPtr dynamicFilterGraph);
    virtual ~AGIPlayable();
private:
    AGIPlayable(AGIPiplineInputPtr input, AGIPiplineIOImagePtr ioImage, AGIFilterGraphPtr filterGraph, AGIFilterDynamicGraphPtr dynamicFilterGraph);

    //region AGIPlayableInterface
public:
    AGIPiplineInputPtr getSourceInputPtr() override;
    AGIPiplineIOImagePtr getTargetIOImagePtr() override;

    AGIFilterGraphPtr getFilterGraphForTime(Milliseconds time) override;
    //endregion AGIPlayableInterface

private:
    AGIPiplineInputPtr m_input;
    AGIPiplineIOImagePtr m_ioImage;

    AGIFilterGraphPtr m_filterGraph;
    AGIFilterDynamicGraphPtr m_dynamicFilterGraph;
};
typedef std::shared_ptr<AGIPlayable> AGIPlayableStaticPtr;


#endif /* AGIPlayable_h */
