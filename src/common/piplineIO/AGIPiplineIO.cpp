//
//  AGIRenderIO.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/2.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIPiplineIO.hpp"
#include <algorithm>


template <class TI>
void AGIPiplineTarget<TI>::endOneProcess() {
    for (auto weakSource : this->getSources()) {
        auto source = weakSource.lock();
	    if (source == nullptr) {
			continue;
	    }
        source->endOneProcess();
    }
}

template <class TI>
std::vector<std::weak_ptr<AGIPiplineSource<TI>>> AGIPiplineTarget<TI>::getSources() {
    return m_sources;
}


template <class SO>
std::vector<std::shared_ptr<AGIPiplineTarget<SO>>> AGIPiplineSource<SO>::getTargets() {
    return m_targets;
}

template <class SO>
void AGIPiplineSource<SO>::addTarget(std::shared_ptr<AGIPiplineTarget<SO>> target) {
    m_targets.push_back(target);
    auto weakThis = std::weak_ptr<AGIPiplineSource<SO>>(this->shared_from_this());
    target->m_sources.push_back(weakThis);
}

template <class SO>
void AGIPiplineSource<SO>::removeTarget(std::shared_ptr<AGIPiplineTarget<SO>> target) {
    auto it = std::find(m_targets.begin(), m_targets.end(), target);
    if (it == m_targets.end()) {
        return;
    }
    m_targets.erase(it);

    auto sit = std::find_if(target->m_sources.begin(), target->m_sources.end(), [&](auto obj){
        if (obj.lock().get() == this) {
            return true;
        }

        return false;
    });
    if (sit == target->m_sources.end()) {
        return;
    }
    target->m_sources.erase(sit);
}

template <class SO>
void AGIPiplineSource<SO>::removeAllTarget() {
    for (auto target : m_targets) {
        auto sit = std::find_if(target->m_sources.begin(), target->m_sources.end(), [&](auto obj){
            if (obj.lock().get() == this) {
                return true;
            }

            return false;
        });
        if (sit == target->m_sources.end()) {
            return;
        }
        target->m_sources.erase(sit);
    }

    m_targets.clear();
}

