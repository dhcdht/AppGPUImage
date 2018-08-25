//
//  AGIFilterGrayscale.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterGrayscale_h
#define AGIFilterGrayscale_h

#include "AGIFilterDefault.h"


class AGIFilterGrayscale : public AGIFilterDefault {
public:
    AGIFilterGrayscale();
    ~AGIFilterGrayscale() override;

public:
    bool setProgress(float progress);

protected:
    bool prepareUniform() override;
protected:
    bool commitUniform() override;

private:
    float m_progress;
};


#endif /* AGIFilterGrayscale_h */
