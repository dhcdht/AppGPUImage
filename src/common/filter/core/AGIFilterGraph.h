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
#include "AGIFilter.h"


class AGIFilterGraph : public AGIPiplineGraph<AGIImagePtr>
{
public:
	AGIFilterGraph();
	~AGIFilterGraph() override;

public:
	AGIFilterPtr getInputAtIndex(int index);
	AGIFilterPtr getOutputAtIndex(int index);
};
typedef std::shared_ptr<AGIFilterGraph> AGIFilterGraphPtr;


#endif /* AGIFilterGraph_h */
