//
//  AGIFilterGraph.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/28.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIFilterGraph.h"


AGIFilterGraph::AGIFilterGraph()
    : m_attachmentInput{nullptr}
    , m_attachmentOutput{nullptr}
{
	
}

AGIFilterGraph::~AGIFilterGraph()
{
    m_attachmentInput = nullptr;
    m_attachmentOutput = nullptr;
}

AGIPiplineInputPtr AGIFilterGraph::getAttachmentInput()
{
    auto lock = this->lockGuardGraph();

    return m_attachmentInput;
}

void AGIFilterGraph::setAttachmentInput(AGIPiplineInputPtr input)
{
    auto lock = this->lockGuardGraph();

    m_attachmentInput = input;
}

AGIPiplineOutputPtr AGIFilterGraph::getAttachmentOutput()
{
    auto lock = this->lockGuardGraph();

    return m_attachmentOutput;
}

void AGIFilterGraph::setAttachmentOutput(AGIPiplineOutputPtr output)
{
    auto lock = this->lockGuardGraph();

    m_attachmentOutput = output;
}

AGIFilterPtr AGIFilterGraph::getGraphSourceAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

    auto sourcePtr = AGIPiplineGraph::getGraphSourceAtIndex(index);
	auto input = static_cast<AGIFilterPtr::element_type*>(sourcePtr.get());
	auto inputPtr = AGIFilterPtr(input);

	return inputPtr;
}

AGIFilterPtr AGIFilterGraph::getGraphTargetAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

    auto targetPtr = AGIPiplineGraph::getGraphTargetAtIndex(index);
	auto output = static_cast<AGIFilterPtr::element_type*>(targetPtr.get());
	auto outputPtr = AGIFilterPtr(output);

	return outputPtr;
}

