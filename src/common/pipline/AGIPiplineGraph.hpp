//
//  AGIPiplineGraph.hpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/27.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIPiplineGraph_hpp
#define AGIPiplineGraph_hpp

#include "AGIPipline.hpp"
#include <deque>
#include <shared_mutex>


template <typename SO, typename TI>
class AGIPiplineGraph
{
public:
	AGIPiplineGraph();
	virtual ~AGIPiplineGraph();

public:
    // write
	bool tryLockGraph();
	void lockGraph();
	void unlockGraph();

    std::unique_lock<std::shared_timed_mutex> lockGuardGraph();

    // read
    bool trySharedLockGraph();
    void lockSharedGraph();
    void unlockSharedGraph();

    std::shared_lock<std::shared_timed_mutex> lockSharedGuardGraph();

public:
	typedef typename AGIPiplineSource<SO>::AGIPiplineSourcePtr AGIPiplineSourcePtr;
	bool addSource(AGIPiplineSourcePtr source);
	void removeSource(AGIPiplineSourcePtr source);
	bool isContainSource(AGIPiplineSourcePtr source);
	int getSourcesCount();
	AGIPiplineSourcePtr getSourceAtIndex(int index);

	typedef typename AGIPiplineTarget<TI>::AGIPiplineTargetPtr AGIPiplineTargetPtr;
	bool addTarget(AGIPiplineTargetPtr target);
	void removeTarget(AGIPiplineTargetPtr target);
	bool isContainTarget(AGIPiplineTargetPtr target);
	int getTargetCount();
	AGIPiplineTargetPtr getTargetAtIndex(int index);

private:
	std::deque<AGIPiplineSourcePtr> m_sources;
	std::deque<AGIPiplineTargetPtr> m_targets;

	std::shared_timed_mutex m_sharedMutex;
};


#endif /* AGIPiplineIOGraph_hpp */
