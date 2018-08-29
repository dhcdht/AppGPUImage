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
	: m_sources{}
	, m_targets{}
{
	
}

template <typename SO, typename TI>
AGIPiplineGraph<SO, TI>::~AGIPiplineGraph()
{
	auto lock = this->lockGuardGraph();

	m_sources.clear();
	m_targets.clear();
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::tryLockGraph()
{
	return m_mutex.try_lock();
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::lockGraph()
{
	m_mutex.lock();
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::unlockGraph()
{
	m_mutex.unlock();
}

template <typename SO, typename TI>
std::unique_lock<std::recursive_mutex> AGIPiplineGraph<SO, TI>::lockGuardGraph()
{
	std::unique_lock<decltype(m_mutex)> lock(m_mutex);

	return std::move(lock);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::addSource(AGIPiplineSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	m_sources.push_back(source);

	return true;
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::removeSource(AGIPiplineSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	m_sources.erase(source);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::isContainSource(AGIPiplineSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	return (m_sources.find(source) != m_sources.end());
}

template <typename SO, typename TI>
int AGIPiplineGraph<SO, TI>::getSourcesCount()
{
	auto lock = this->lockGuardGraph();

	return m_sources.size();
}

template <typename SO, typename TI>
typename AGIPiplineGraph<SO, TI>::AGIPiplineSourcePtr AGIPiplineGraph<SO, TI>::getSourceAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	return m_sources[index];
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::addTarget(AGIPiplineTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	m_targets.push_back(target);

	return true;
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::removeTarget(AGIPiplineTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	m_targets.erase(target);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::isContainTarget(AGIPiplineTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	return (m_targets.find(target) != m_targets.end());
}

template <typename SO, typename TI>
int AGIPiplineGraph<SO, TI>::getTargetCount()
{
	auto lock = this->lockGuardGraph();

	return m_targets.size();
}

template <typename SO, typename TI>
typename AGIPiplineGraph<SO, TI>::AGIPiplineTargetPtr AGIPiplineGraph<SO, TI>::getTargetAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	return m_targets[index];
}

