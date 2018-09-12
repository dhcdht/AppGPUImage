//
//  AGIPiplineConnection.hpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/9/12.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineConnection_hpp
#define AGIPiplineConnection_hpp

#include "AGIPipline.hpp"


template <typename SO, typename TI>
class AGIPiplineConnection {
public:
    typedef std::shared_ptr<AGIPiplineSource<SO>> AGIPiplineSourcePtr;
    typedef std::shared_ptr<AGIPiplineTarget<TI>> AGIPiplineTargetPtr;

public:
    AGIPiplineConnection(AGIPiplineSourcePtr source, AGIPiplineTargetPtr target)
        : m_source(source)
        , m_target(target)
    {
        m_source->addTarget(m_target);
    };
    virtual ~AGIPiplineConnection()
    {
        m_source->removeTarget(m_target);

        m_source = nullptr;
        m_target = nullptr;
    };

private:
    AGIPiplineSourcePtr m_source;
    AGIPiplineTargetPtr m_target;
};


#endif /* AGIPiplineConnection_hpp */
