#include "NotifyEventDispatcher.h"

#include <cstdint>

#include <sys/inotify.h>
#include <unistd.h>

#include "NotifyEvent.h"

namespace Agent::Notify
{

NotifyEventDispatcher::NotifyEventDispatcher(std::unique_ptr<NotifyManager> manager, std::unique_ptr<NotifyEventHandler> handler)
{
	m_manager = std::move(manager);
	m_handler = std::move(handler);
	m_continueReading = true;

	if (m_manager->Configuration()->m_isMultithreaded)
	{
		initializeThreadPool();
	}
}

void NotifyEventDispatcher::initializeThreadPool()
{
	std::cout << "Running in multi-threaded mode." << std::endl;
	m_threadPool = std::make_unique<thread_pool::static_pool>();
}

void NotifyEventDispatcher::BeginEventLoop()
{
	if (!m_manager->Descriptor()->IsValid())
	{
		return;
	}

	constexpr auto BUFFER_SIZE = (10 * (sizeof(struct inotify_event) + NAME_MAX + 1));
	std::array<char, BUFFER_SIZE> readBuffer;
	while (m_continueReading)
	{
		auto bytesRead = read(m_manager->Descriptor()->Value(), readBuffer.data(), readBuffer.size());
		if (bytesRead == 0)
		{
			std::cerr << "No bytes read from " << m_manager->Descriptor()->Value() << std::endl;
		}
		if (bytesRead == -1)
		{
			perror("read");
			break;
		}

		for (auto *bufferStart = readBuffer.data(); bufferStart < readBuffer.data() + bytesRead && m_continueReading; /*Empty*/)
		{
			inotify_event *pEvent = (inotify_event *)bufferStart;

			std::cout << "Read event for " << pEvent->name << " - dispatching" << std::endl;
			dispatchEvent(pEvent);

			bufferStart += sizeof(inotify_event) + pEvent->len;
		}
	}
}

void NotifyEventDispatcher::StopEventLoop()
{
	m_continueReading = false;
}

void NotifyEventDispatcher::dispatchEvent(const inotify_event* pEvent)
{
	std::cout << "Read event for watch " << pEvent->wd << std::endl;

	if (pEvent->len <= 0)
	{
		std::cerr << "No file name associated with event. Watch descriptor = " << pEvent->wd << std::endl;
		return;
	}

	if (pEvent->mask & IN_CLOSE_WRITE)
	{
		std::string fileName((char*)pEvent->name);
		auto config = m_manager->Configuration();
		std::shared_ptr<NotifyEvent> notifyEvent(new NotifyEvent(config, fileName, pEvent->wd, pEvent->mask));

		if (m_manager->Configuration()->m_isMultithreaded && m_threadPool != nullptr)
		{
			m_threadPool->enqueue([this](std::shared_ptr<NotifyEvent> notifyEvent)
				{ m_handler->Handle(notifyEvent); },
				notifyEvent);
		}
		else
		{
			m_handler->Handle(notifyEvent);
		}
	}
}

}
