//
//  AGIFilterRangedGraph.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/11.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterRangedGraph.h"


AGIFilterRangedGraph::AGIFilterRangedGraph(Milliseconds beginTime, Milliseconds endTime, AGIFilterGraphPtr graph)
    : m_beginTime{beginTime}
    , m_endTime{endTime}
    , m_graph{graph}
{

}

AGIFilterRangedGraph::~AGIFilterRangedGraph()
{
    m_beginTime = Milliseconds(0);
    m_endTime = Milliseconds(0);
    m_graph = nullptr;
}

Milliseconds AGIFilterRangedGraph::getBeginTime()
{
    return m_beginTime;
}


Milliseconds AGIFilterRangedGraph::getEndTime()
{
    return m_endTime;
}

AGIFilterGraphPtr AGIFilterRangedGraph::getGraph()
{
    return m_graph;
}

