#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "NotifyEvent.h"

namespace Agent::Notify
{

class NotifyEventHandler
{
public:
	NotifyEventHandler(const std::string& language);
	virtual ~NotifyEventHandler() = default;

	const std::string& Language() const;

	virtual void Handle(std::shared_ptr<NotifyEvent> event) = 0;
private:
	std::string m_language;
};

}
