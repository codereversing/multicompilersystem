#include "NotifyEventHandlerTopmost.h"

namespace Agent::Notify
{

NotifyEventHandlerTopmost::NotifyEventHandlerTopmost() : NotifyEventHandler("none")
{
}

void NotifyEventHandlerTopmost::Handle(std::shared_ptr<NotifyEvent> event)
{
	const std::string& type = event->Language();
	auto handler = m_dispatchTable.find(type);
	if (handler == m_dispatchTable.end())
	{
		std::cerr << "No handler found for " << event->Language() << " files" << std::endl;
		return;
	}

	handler->second->Handle(event);
}

bool NotifyEventHandlerTopmost::AddHandler(std::unique_ptr<NotifyEventHandler> handler)
{
	const std::string& language = handler->Language();
	if (m_dispatchTable.find(handler->Language()) != m_dispatchTable.end())
	{
		std::cerr << "Handler for " << language << " already exists." << std::endl;
		return false;
	}

	std::cout << "Adding handler for " << language << std::endl;
	m_dispatchTable.insert({ handler->Language(), std::move(handler) });
	return true;
}

bool NotifyEventHandlerTopmost::RemoveHandler(std::unique_ptr<NotifyEventHandler> handler)
{
	const std::string& language = handler->Language();
	if (m_dispatchTable.find(handler->Language()) == m_dispatchTable.end())
	{
		std::cerr << "Handler for " << language << " doesnt exist." << std::endl;
		return false;
	}

	std::cout << "Removing handler for " << language << std::endl;
	m_dispatchTable.erase(handler->Language());
	return true;
}

}
