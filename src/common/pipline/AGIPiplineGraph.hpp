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
#include <mutex>


template <typename SO, typename TI>
class AGIPiplineGraph
{
public:
	AGIPiplineGraph();
	virtual ~AGIPiplineGraph();

public:
	bool tryLockGraph();
	void lockGraph();
	void unlockGraph();

	std::unique_lock<std::recursive_mutex> lockGuardGraph();

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

    // todo: use read write lock for perfermance
	std::recursive_mutex m_mutex;
};


#endif /* AGIPiplineIOGraph_hpp */
