//
//  AGIFilterRangedGraph.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/11.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterRangedGraph_h
#define AGIFilterRangedGraph_h

#include "AGIFilterGraph.h"
#include "util/bgfxUtils.h"


class AGIFilterRangedGraph
{
public:
    AGIFilterRangedGraph(Milliseconds beginTime, Milliseconds endTime, AGIFilterGraphPtr graph);
    virtual ~AGIFilterRangedGraph();

public:
    Milliseconds getBeginTime();
    Milliseconds getEndTime();
    AGIFilterGraphPtr getGraph();

private:
    Milliseconds m_beginTime;
    Milliseconds m_endTime;
    AGIFilterGraphPtr m_graph;
};
typedef std::shared_ptr<AGIFilterRangedGraph> AGIFilterRangedGraphPtr;


#endif /* AGIFilterRangedGraph_h */
