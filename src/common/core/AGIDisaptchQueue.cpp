//
//  SDKTest.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/26.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "AGIDispatchQueue.h"


AGIDispatchQueue::AGIDispatchQueue(std::string name, size_t threadCount)
	: m_name(name)
	, m_threads(threadCount)
	, m_quit(false)
{
	for (size_t i = 0; i < m_threads.size(); ++i)
	{
		m_threads[i] = std::thread(std::bind(&AGIDispatchQueue::dispatchQueueHandle, this));
	}
}

AGIDispatchQueue::~AGIDispatchQueue()
{
	m_quit = true;
	m_conditionVariable.notify_all();

	for (int i = 0; i < m_threads.size(); ++i)
	{
		if (m_threads[i].joinable())
		{
			m_threads[i].join();
		}
	}
}

void AGIDispatchQueue::dispatch(const Operation& operation)
{
	std::unique_lock<decltype(m_lock)> lock(m_lock);
	m_operations.push(operation);
	lock.unlock();

	m_conditionVariable.notify_all();
}

void AGIDispatchQueue::dispatchQueueHandle()
{
	std::unique_lock<decltype(m_lock)> lock(m_lock);

	do
	{
		m_conditionVariable.wait(lock, [&]() {
			return (m_operations.size() > 0 || m_quit);
		});

		if (m_operations.size() > 0 && !m_quit)
		{
			auto operation = std::move(m_operations.front());
			m_operations.pop();

			lock.unlock();
			operation();
			lock.lock();
		}
	}
	while (!m_quit);
}


