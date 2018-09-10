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
	typedef typename AGIPiplineSource<SO>::AGIPiplineSourcePtr AGIPiplineGraphSourcePtr;
	bool addGraphSource(AGIPiplineGraphSourcePtr source);
	void removeGraphSource(AGIPiplineGraphSourcePtr source);
	bool isContainGraphSource(AGIPiplineGraphSourcePtr source);
	int getGraphSourcesCount();
	AGIPiplineGraphSourcePtr getGraphSourceAtIndex(int index);

	typedef typename AGIPiplineTarget<TI>::AGIPiplineTargetPtr AGIPiplineGraphTargetPtr;
	bool addGraphTarget(AGIPiplineGraphTargetPtr target);
	void removeGraphTarget(AGIPiplineGraphTargetPtr target);
	bool isContainGraphTarget(AGIPiplineGraphTargetPtr target);
	int getGraphTargetCount();
	AGIPiplineGraphTargetPtr getGraphTargetAtIndex(int index);

private:
	std::deque<AGIPiplineGraphSourcePtr> m_graphSources;
	std::deque<AGIPiplineGraphTargetPtr> m_graphTargets;

	// c++14 加入的 shared_timed_mutex 和 c++17 加入的 shared_mutex 都被普遍认为性能有问题，
	// 这也导致了他们加入标准的顺序很奇怪，
	// 另外，他们都没有 recursive 递归锁的功能，
	// 所以这里只使用了普通的递归锁，而不是读写锁。
	std::recursive_mutex m_graphMutex;
};


#endif /* AGIPiplineIOGraph_hpp */
