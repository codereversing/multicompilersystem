#include "NotifyEventHandler.h"

#include <iostream>
#include <string>

namespace Agent::Notify
{

NotifyEventHandler::NotifyEventHandler(const std::string& type)
{
	m_language = type;
}

const std::string& NotifyEventHandler::Language() const
{
	return m_language;
}

}
