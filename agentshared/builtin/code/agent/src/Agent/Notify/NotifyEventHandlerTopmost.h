#pragma once

#include <string>
#include <unordered_map>

#include "NotifyEventHandler.h"

namespace Agent::Notify
{

class NotifyEventHandlerTopmost : public NotifyEventHandler
{
public:
	NotifyEventHandlerTopmost();
	void Handle(std::shared_ptr<NotifyEvent> event) override;

	bool AddHandler(std::unique_ptr<NotifyEventHandler> handler);
	bool RemoveHandler(std::unique_ptr<NotifyEventHandler> handler);
private:
	std::unordered_map<std::string /*Language*/, std::unique_ptr<NotifyEventHandler> /*Handler*/> m_dispatchTable;
};

}
