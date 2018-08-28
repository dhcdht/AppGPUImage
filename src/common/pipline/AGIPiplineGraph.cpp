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
{
	
}

template <typename SO, typename TI>
AGIPiplineGraph<SO, TI>::~AGIPiplineGraph()
{
	
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::addSource(AGIPiplineSourcePtr source)
{
	m_sources.insert(source);

	return true;
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::removeSource(AGIPiplineSourcePtr source)
{
	m_sources.erase(source);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::isContainSource(AGIPiplineSourcePtr source)
{
	return (m_sources.find(source) != m_sources.end());
}

template <typename SO, typename TI>
int AGIPiplineGraph<SO, TI>::getSourcesCount()
{
	return m_sources.size();
}

template <typename SO, typename TI>
typename AGIPiplineGraph<SO, TI>::AGIPiplineSourcePtr AGIPiplineGraph<SO, TI>::getSourceAtIndex(int index)
{
	return m_sources[index];
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::addTarget(AGIPiplineTargetPtr target)
{
	m_targets.insert(target);

	return true;
}

template <typename SO, typename TI>
void AGIPiplineGraph<SO, TI>::removeTarget(AGIPiplineTargetPtr target)
{
	m_targets.erase(target);
}

template <typename SO, typename TI>
bool AGIPiplineGraph<SO, TI>::isContainTarget(AGIPiplineTargetPtr target)
{
	return (m_targets.find(target) != m_targets.end());
}

template <typename SO, typename TI>
int AGIPiplineGraph<SO, TI>::getTargetCount()
{
	return m_targets.size();
}

template <typename SO, typename TI>
typename AGIPiplineGraph<SO, TI>::AGIPiplineTargetPtr AGIPiplineGraph<SO, TI>::getTargetAtIndex(int index)
{
	return m_targets[index];
}

