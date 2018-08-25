//
//  AGIFilterTwoPass.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/5.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIFilterTwoPass_h
#define AGIFilterTwoPass_h

#include "AGIFilterDefault.h"


class AGIFilterTwoPass : public AGIFilterDefault {
public:
    AGIFilterTwoPass();
    ~AGIFilterTwoPass() override;

public:
    int getTargetInputCount() override {
        return 2;
    };

protected:
    bool prepareUniform() override;
protected:
    bool commitUniform() override;
};


#endif /* AGIFilterTwoPass_h */
