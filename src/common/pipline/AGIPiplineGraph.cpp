//
//  AGIPiplineGraph.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/8/27.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineGraph.hpp"


template <typename GN>
AGIPiplineGraph<GN>::AGIPiplineGraph()
	: m_graphSources{}
	, m_graphTargets{}
	, m_graphMutex{}
{
	
}

template <typename GN>
AGIPiplineGraph<GN>::~AGIPiplineGraph()
{
	auto lock = this->lockGuardGraph();

	m_graphSources.clear();
	m_graphTargets.clear();
}

template <typename GN>
bool AGIPiplineGraph<GN>::tryLockGraph()
{
	return m_graphMutex.try_lock();
}

template <typename GN>
void AGIPiplineGraph<GN>::lockGraph()
{
	m_graphMutex.lock();
}

template <typename GN>
void AGIPiplineGraph<GN>::unlockGraph()
{
	m_graphMutex.unlock();
}

template <typename GN>
std::unique_lock<std::recursive_mutex> AGIPiplineGraph<GN>::lockGuardGraph()
{
	std::unique_lock<decltype(m_graphMutex)> lock(m_graphMutex);

	return std::move(lock);
}

template <typename GN>
bool AGIPiplineGraph<GN>::addGraphSource(AGIPiplineGraphSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	m_graphSources.push_back(source);

	return true;
}

template <typename GN>
void AGIPiplineGraph<GN>::removeGraphSource(AGIPiplineGraphSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	m_graphSources.erase(source);
}

template <typename GN>
bool AGIPiplineGraph<GN>::isContainGraphSource(AGIPiplineGraphSourcePtr source)
{
	auto lock = this->lockGuardGraph();

	return (m_graphSources.find(source) != m_graphSources.end());
}

template <typename GN>
int AGIPiplineGraph<GN>::getGraphSourcesCount()
{
	auto lock = this->lockGuardGraph();

	return m_graphSources.size();
}

template <typename GN>
typename AGIPiplineGraph<GN>::AGIPiplineGraphSourcePtr AGIPiplineGraph<GN>::getGraphSourceAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	return m_graphSources[index];
}

template <typename GN>
bool AGIPiplineGraph<GN>::addGraphTarget(AGIPiplineGraphTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	m_graphTargets.push_back(target);

	return true;
}

template <typename GN>
void AGIPiplineGraph<GN>::removeGraphTarget(AGIPiplineGraphTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	m_graphTargets.erase(target);
}

template <typename GN>
bool AGIPiplineGraph<GN>::isContainGraphTarget(AGIPiplineGraphTargetPtr target)
{
	auto lock = this->lockGuardGraph();

	return (m_graphTargets.find(target) != m_graphTargets.end());
}

template <typename GN>
int AGIPiplineGraph<GN>::getGraphTargetCount()
{
	auto lock = this->lockGuardGraph();

	return m_graphTargets.size();
}

template <typename GN>
typename AGIPiplineGraph<GN>::AGIPiplineGraphTargetPtr AGIPiplineGraph<GN>::getGraphTargetAtIndex(int index)
{
	auto lock = this->lockGuardGraph();

	return m_graphTargets[index];
}

template <typename GN>
void AGIPiplineGraph<GN>::endOneProcess()
{
    auto lock = this->lockGuardGraph();

    for (auto target : m_graphTargets) {
        target->endOneProcess();
    }
}

template <typename GN>
int AGIPiplineGraph<GN>::getSourceOutputCount()
{
    auto lock = this->lockGuardGraph();

    int outputCount = 0;
    for (auto target : m_graphTargets) {
        outputCount += target->getSourceOutputCount();
    }

    return outputCount;
}

template <typename GN>
std::vector<GN> AGIPiplineGraph<GN>::pullOutputs()
{
    auto lock = this->lockGuardGraph();

    std::vector<GN> outputs;
    for (auto target : m_graphTargets) {
        auto targetOutputs = target->pullOutputs();
        for (auto output : targetOutputs) {
            outputs.push_back(output);
        }
    }

    return outputs;
}

template <typename GN>
int AGIPiplineGraph<GN>::getTargetInputCount()
{
    auto lock = this->lockGuardGraph();

    int inputCount = 0;
    for (auto source : m_graphSources) {
        inputCount += source->getTargetInputCount();
    }

    return inputCount;
}

template <typename GN>
bool AGIPiplineGraph<GN>::processTarget()
{
    auto lock = this->lockGuardGraph();

    bool ret = true;
    for (auto target : m_graphTargets) {
        ret &= target->processTarget();
    }

    return ret;
}

