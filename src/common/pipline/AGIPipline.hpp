//
//  AGIPipline.hpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPipline_hpp
#define AGIPipline_hpp

#include <iostream>
#include <memory>
#include <bgfx/bgfx.h>
#include <vector>


template <class SO>
class AGIPiplineSource;
template <class TI>
class AGIPiplineTarget {
public:
    typedef std::shared_ptr<AGIPiplineTarget<TI>> AGIPiplineTargetPtr;
    typedef std::weak_ptr<AGIPiplineSource<TI>> WeakPtrSource;
public:
    virtual int getTargetInputCount() = 0;

    // pull
public:
    virtual bool processTarget() = 0;
    virtual void endOneProcess();

//    // push
//public:
//    virtual bool setNextInputs(std::vector<TI> inputs, AGIPiplineSource<TI>* fromSource);
//    virtual bool onAllInputsReady(std::vector<TI> inputs) = 0;
//protected:
//    std::vector<std::vector<TI>> m_tempSourceInputs;

public:
    virtual std::vector<WeakPtrSource> getSources();

protected:
    friend class AGIPiplineSource<TI>;
    std::vector<WeakPtrSource> m_sources;
};


template <class SO>
class AGIPiplineSource : public std::enable_shared_from_this<AGIPiplineSource<SO>> {
public:
    typedef std::shared_ptr<AGIPiplineSource<SO>> AGIPiplineSourcePtr;
    typedef std::shared_ptr<AGIPiplineTarget<SO>> SharedPtrTarget;
public:
    virtual int getSourceOutputCount() = 0;

    // pull
public:
    virtual std::vector<SO> pullOutputs() = 0;
    virtual void endOneProcess() {};

//    // push
//public:
//    virtual bool processSource() = 0;

public:
    virtual std::vector<SharedPtrTarget> getTargets();
    virtual void addTarget(SharedPtrTarget target);
    virtual void removeTarget(SharedPtrTarget target);
    virtual void removeAllTarget();
protected:
    std::vector<SharedPtrTarget> m_targets;
};


template <class SO, class TI>
class AGIPiplineNode : public AGIPiplineSource<SO>, public AGIPiplineTarget<TI> {
public:
    typedef std::shared_ptr<AGIPiplineNode<SO, TI>> AGIPiplineNodePtr;
    // pull
public:
    void endOneProcess() override { AGIPiplineTarget<TI>::endOneProcess(); };
};


#endif /* AGIPipline_hpp */
