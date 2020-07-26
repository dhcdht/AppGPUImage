//
//  AGIPlayable.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/12.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPlayable.h"


AGIPlayable::AGIPlayable(AGIPiplineInputPtr input, AGIPiplineIOImagePtr ioImage, AGIFilterGraphPtr filterGraph)
    : AGIPlayable(input, ioImage, filterGraph, nullptr)
{

}

AGIPlayable::AGIPlayable(AGIPiplineInputPtr input, AGIPiplineIOImagePtr ioImage, AGIFilterDynamicGraphPtr dynamicFilterGraph)
    : AGIPlayable(input, ioImage, nullptr, dynamicFilterGraph)
{

}

AGIPlayable::~AGIPlayable()
{
    m_input = nullptr;
    m_ioImage = nullptr;
    m_filterGraph = nullptr;
    m_dynamicFilterGraph = nullptr;
}

AGIPlayable::AGIPlayable(AGIPiplineInputPtr input, AGIPiplineIOImagePtr ioImage, AGIFilterGraphPtr filterGraph, AGIFilterDynamicGraphPtr dynamicFilterGraph)
    : m_input(input)
    , m_ioImage(ioImage)
    , m_filterGraph(filterGraph)
    , m_dynamicFilterGraph(dynamicFilterGraph)
{
    
}

AGIPiplineInputPtr AGIPlayable::getSourceInputPtr()
{
    return m_input;
}

AGIPiplineIOImagePtr AGIPlayable::getTargetIOImagePtr()
{
    return m_ioImage;
}

AGIFilterGraphPtr AGIPlayable::getFilterGraphForTime(Milliseconds time)
{
    if (m_filterGraph) {
        return m_filterGraph;
    }

    // todo: front?
    return m_dynamicFilterGraph->getFilterGraphFroTime(time).front();
}

