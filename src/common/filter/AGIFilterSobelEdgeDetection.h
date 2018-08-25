//
//  AGIFilterSobelEdgeDetection.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/5.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterSobelEdgeDetection_h
#define AGIFilterSobelEdgeDetection_h

#include "AGIFilterDefault.h"


class AGIFilterSobelEdgeDetection : public AGIFilterDefault {
public:
    AGIFilterSobelEdgeDetection();
    ~AGIFilterSobelEdgeDetection() override;

public:
    bool setEdgePixelWidth(float pixelWidth);
    bool setEdgePixelHeight(float pixelHeight);
    bool setEdgeStrength(float edgeStrength);

protected:
    bool prepareUniform() override;
protected:
    bool commitUniform() override;

private:
    float m_pixelWidth;
    float m_pixelHeight;
    float m_edgeStrength;
};


#endif /* AGIFilterSobelEdgeDetection_h */
