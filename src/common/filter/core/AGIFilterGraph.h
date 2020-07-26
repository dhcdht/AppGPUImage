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
#include "IO/input/AGIPiplineInput.h"
#include "IO/output/AGIPiplineOutput.h"
#include "AGIFilterSettingInterface.h"


class AGIFilterGraph : public AGIPiplineGraph<AGIImagePtr>, public AGIFilterSettingContainer
{
public:
	AGIFilterGraph();
	~AGIFilterGraph() override;

public:
    AGIPiplineInputPtr getAttachmentInput();
    void setAttachmentInput(AGIPiplineInputPtr input);

    AGIPiplineOutputPtr getAttachmentOutput();
    void setAttachmentOutput(AGIPiplineOutputPtr output);

    //region AGIPiplineGraph
public:
    AGIFilterPtr getGraphSourceAtIndex(int index);  // overload
    AGIFilterPtr getGraphTargetAtIndex(int index);  // overload
    //endregion AGIPiplineGraph

private:
    AGIPiplineInputPtr m_attachmentInput;
    AGIPiplineOutputPtr m_attachmentOutput;
};
typedef std::shared_ptr<AGIFilterGraph> AGIFilterGraphPtr;


#endif /* AGIFilterGraph_h */
