//
//  AGIFilterGraph.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/28.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterGraph_h
#define AGIFilterGraph_h

#include "pipline/AGIPiplineGraph.hpp"
#include "pipline/AGIPiplineGraph.cpp"
#include "IO/AGIImage.h"
#include "IO/input/AGIPiplineInput.h"
#include "IO/output/AGIPiplineOutput.h"


class AGIFilterGraph : public AGIPiplineGraph<AGIImagePtr, AGIImagePtr>
{
public:
	AGIFilterGraph();
	~AGIFilterGraph() override;

public:
	AGIPiplineInputPtr getInputAtIndex(int index);
	AGIPiplineOutputPtr getOutputAtIndex(int index);
};
typedef std::shared_ptr<AGIFilterGraph> AGIFilterGraphPtr;


#endif /* AGIFilterGraph_h */
