//
//  AGIFilterTransform.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterTransform_h
#define AGIFilterTransform_h

#include "AGIFilter.h"


class AGIFilterTransform : public AGIFilter {
public:
    ~AGIFilterTransform() override;
    AGIFilterTransform();

public:
    bool setTransform(float mat[4][4]);

protected:
    bool prepareUniform() override;
protected:
    bool commitUniform() override;

private:
    float m_transform[4][4];
};


#endif /* AGIFilterTransform_h */
