#pragma once

#include <memory>
#include <string>

#include "NotifyConfiguration.h"
#include "NotifyDescriptor.h"
#include "NotifyWatchDescriptor.h"

namespace Agent::Notify
{

class NotifyWatch
{
public:
	NotifyWatch(std::shared_ptr<NotifyDescriptor> descriptor, std::shared_ptr<NotifyConfiguration> configuration);
	~NotifyWatch();

	bool Add();
	bool Remove();
	bool IsActive() const;

private:
	std::shared_ptr<NotifyDescriptor> m_notifyDescriptor;
	std::shared_ptr<NotifyWatchDescriptor> m_watchDescriptor;
	std::string m_targetPath;
};

}
