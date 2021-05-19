#pragma once

#include <atomic>
#include <memory>

#include "NotifyEventHandler.h"
#include "NotifyManager.h"

#include "static_pool.hpp"

struct inotify_event;

namespace Agent::Notify
{

class NotifyEventDispatcher
{
public:
	NotifyEventDispatcher(std::unique_ptr<NotifyManager> manager, std::unique_ptr<NotifyEventHandler> handler);

	void BeginEventLoop();
	void StopEventLoop();

private:
	void initializeThreadPool();
	void dispatchEvent(const inotify_event *pEvent);

	std::atomic_bool m_continueReading;

	std::unique_ptr<NotifyManager> m_manager;
	std::unique_ptr<NotifyEventHandler> m_handler;
	std::unique_ptr<thread_pool::static_pool> m_threadPool;
};

}
