//
//  AGIPiplineGraph.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/27.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineGraph.hpp"


template <typename SO, typename TI>
AGIPiplineGraph<SO, TI>::AGIPiplineGraph()
	: m_graphSources{}
	, m_graphTargets{}
	, m_graphMutex{}
{
	
}

template <typename SO, typename TI>
AGIPiplineGraph<SO, TI>::~AGIPiplineGraph()
{
	auto lock = this->lockGuardGraph();

	m_graphSources.clear();
	m_graphTargets.clear();
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::tryLockGraph()
{
	return m_graphMutex.try_lock();
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::lockGraph()
{
	m_graphMutex.lock();
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::unlockGraph()
{
	m_graphMutex.unlock();
}

template <typename SO, typename TI>
std::unique_lock<std::recursive_mutex> AGIPiplineGraph<SO, TI>::lockGuardGraph()
{
	std::unique_lock<decltype(m_graphMutex)> lock(m_graphMutex);

	return std::move(lock);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::addGraphSource(AGIPiplineGraphSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	m_graphSources.push_back(source);

	return true;
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::removeGraphSource(AGIPiplineGraphSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	m_graphSources.erase(source);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::isContainGraphSource(AGIPiplineGraphSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	return (m_graphSources.find(source) != m_graphSources.end());
}

template <typename SO, typename TI>
int AGIPiplineGraph<SO, TI>::getGraphSourcesCount()
{
	auto lock = this->lockGuardGraph();

	return m_graphSources.size();
}

template <typename SO, typename TI>
typename AGIPiplineGraph<SO, TI>::AGIPiplineGraphSourcePtr AGIPiplineGraph<SO, TI>::getGraphSourceAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	return m_graphSources[index];
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::addGraphTarget(AGIPiplineGraphTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	m_graphTargets.push_back(target);

	return true;
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::removeGraphTarget(AGIPiplineGraphTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	m_graphTargets.erase(target);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::isContainGraphTarget(AGIPiplineGraphTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	return (m_graphTargets.find(target) != m_graphTargets.end());
}

template <typename SO, typename TI>
int AGIPiplineGraph<SO, TI>::getGraphTargetCount()
{
	auto lock = this->lockGuardGraph();

	return m_graphTargets.size();
}

template <typename SO, typename TI>
typename AGIPiplineGraph<SO, TI>::AGIPiplineGraphTargetPtr AGIPiplineGraph<SO, TI>::getGraphTargetAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	return m_graphTargets[index];
}

