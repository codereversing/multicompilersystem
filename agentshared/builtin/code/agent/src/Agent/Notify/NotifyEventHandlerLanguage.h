#pragma once

#include "NotifyEventHandler.h"

namespace Agent::Notify
{

class NotifyEventHandlerLanguage : public NotifyEventHandler
{
public:
	NotifyEventHandlerLanguage(const std::string& language);
	void Handle(std::shared_ptr<NotifyEvent> event) override;
};

}
