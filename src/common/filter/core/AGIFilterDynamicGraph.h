//
//  AGIFilterDynamicGraph.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/11.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterDynamicGraph_h
#define AGIFilterDynamicGraph_h

#include "AGIFilterRangedGraph.h"
#include <list>


class AGIFilterDynamicGraph
{
public:
    AGIFilterDynamicGraph();
    virtual ~AGIFilterDynamicGraph();

public:
    std::vector<AGIFilterGraphPtr> getFilterGraphFroTime(Milliseconds time);

    AGIFilterRangedGraphPtr addRangedGraph(Milliseconds beginTime, Milliseconds endTime, AGIFilterGraphPtr graph);
    bool isContainedRangedGraph(AGIFilterRangedGraphPtr graph);
    bool removeRangedGraph(AGIFilterRangedGraphPtr graph);

private:
    std::list<AGIFilterRangedGraphPtr> m_rangedGraphs;
    std::recursive_mutex m_graphsMutex;
};


#endif /* AGIFilterDynamicGraph_h */
