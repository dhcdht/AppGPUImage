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

AGIFilterPtr AGIFilterGraph::getInputAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	auto sourcePtr = this->getGraphSourceAtIndex(index);
	auto input = static_cast<AGIFilterPtr::element_type*>(sourcePtr.get());
	auto inputPtr = AGIFilterPtr(input);

	return inputPtr;
}

AGIFilterPtr AGIFilterGraph::getOutputAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	auto targetPtr = this->getGraphTargetAtIndex(index);
	auto output = static_cast<AGIFilterPtr::element_type*>(targetPtr.get());
	auto outputPtr = AGIFilterPtr(output);

	return outputPtr;
}

