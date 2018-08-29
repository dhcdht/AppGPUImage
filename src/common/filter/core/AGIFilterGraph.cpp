//
//  AGIFilterGraph.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/28.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterGraph.h"


AGIFilterGraph::AGIFilterGraph()
{
	
}

AGIFilterGraph::~AGIFilterGraph()
{

}

AGIPiplineInputPtr AGIFilterGraph::getInputAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	auto sourcePtr = this->getSourceAtIndex(index);
	auto input = static_cast<AGIPiplineInputPtr::element_type*>(sourcePtr.get());
	auto inputPtr = AGIPiplineInputPtr(input);

	return inputPtr;
}

AGIPiplineOutputPtr AGIFilterGraph::getOutputAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	auto targetPtr = this->getTargetAtIndex(index);
	auto output = static_cast<AGIPiplineOutputPtr::element_type*>(targetPtr.get());
	auto outputPtr = AGIPiplineOutputPtr(output);

	return outputPtr;
}

