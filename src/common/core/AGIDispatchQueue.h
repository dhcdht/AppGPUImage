//
//  AGIDispatchQueue.h
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/7/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#ifndef AGIDispatchQueue_h
#define AGIDispatchQueue_h

#include <iostream>
#include <functional>
#include <mutex>
#include <vector>
#include <thread>
#include <queue>


class AGIDispatchQueue
{
public:
	AGIDispatchQueue(std::string name, size_t threadCount = 1);
	virtual ~AGIDispatchQueue();

public:
	typedef std::function<void()> Operation;

	void dispatch(const Operation& operation);
	void syncDispatch(const Operation& operation);
	void syncCancelAllOperation();

public:
	// 不允许拷贝复制等特殊操作
	AGIDispatchQueue(const AGIDispatchQueue& rhs) = delete;
	AGIDispatchQueue(AGIDispatchQueue&& rhs) = delete;
	AGIDispatchQueue& operator=(const AGIDispatchQueue& rhs) = delete;
	AGIDispatchQueue& operator=(AGIDispatchQueue&& rhs) = delete;

private:
	void dispatchQueueHandle(void);

private:
	std::string m_name;
	std::mutex m_lock;
	std::vector<std::thread> m_threads;
	std::queue<Operation> m_operations;
	Operation m_syncOperation;
	std::mutex m_syncMutex;
	std::condition_variable m_syncConditionVariable;
	std::condition_variable m_conditionVariable;
	bool m_quit;
};


#endif /* AGIDispatchQueue_h */
