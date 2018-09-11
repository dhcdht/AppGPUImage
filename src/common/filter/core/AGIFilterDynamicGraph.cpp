//
//  AGIFilterDynamicGraph.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/11.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterDynamicGraph.h"


AGIFilterDynamicGraph::AGIFilterDynamicGraph()
    : m_rangedGraphs{}
    , m_graphsMutex{}
{

}

AGIFilterDynamicGraph::~AGIFilterDynamicGraph()
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_graphsMutex);
    
    m_rangedGraphs.clear();
}

std::vector<AGIFilterGraphPtr> AGIFilterDynamicGraph::getFilterGraphFroTime(Milliseconds time)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_graphsMutex);

    std::vector<AGIFilterGraphPtr> ret;
    for (auto it = m_rangedGraphs.begin(); it != m_rangedGraphs.end(); it++) {
        auto itRangedGraphPtr = (*it);
        if (itRangedGraphPtr->getBeginTime() <= time
            && time <= itRangedGraphPtr->getEndTime()) {
            ret.push_back(itRangedGraphPtr->getGraph());

            // todo: 计算 graph settings？
        }
    }

    return ret;
}

AGIFilterRangedGraphPtr AGIFilterDynamicGraph::addRangedGraph(Milliseconds beginTime, Milliseconds endTime, AGIFilterGraphPtr graph)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_graphsMutex);

    auto rangedGraphPtr = std::make_shared<AGIFilterRangedGraph>(beginTime, endTime, graph);

    for (auto it = m_rangedGraphs.begin(); it != m_rangedGraphs.end(); it++) {
        auto itRangedGraphPtr = (*it);
        if (itRangedGraphPtr->getBeginTime() > beginTime) {
            m_rangedGraphs.insert(it, rangedGraphPtr);

            return rangedGraphPtr;
        }
    }

    m_rangedGraphs.push_back(rangedGraphPtr);
    return rangedGraphPtr;
}

bool AGIFilterDynamicGraph::isContainedRangedGraph(AGIFilterRangedGraphPtr graph)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_graphsMutex);

    for (auto it = m_rangedGraphs.begin(); it != m_rangedGraphs.end(); it++) {
        auto itRangedGraphPtr = (*it);
        if (itRangedGraphPtr == graph) {
            return true;
        }
    }

    return false;
}

bool AGIFilterDynamicGraph::removeRangedGraph(AGIFilterRangedGraphPtr graph)
{
    auto lock = std::unique_lock<std::recursive_mutex>(m_graphsMutex);

    for (auto it = m_rangedGraphs.begin(); it != m_rangedGraphs.end(); it++) {
        auto itRangedGraphPtr = (*it);
        if (itRangedGraphPtr == graph) {
            m_rangedGraphs.erase(it);
            return true;
        }
    }

    return false;
}

