#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "NotifyConfiguration.h"
#include "NotifyDescriptor.h"
#include "NotifyWatch.h"

namespace Agent::Notify
{

class NotifyManager final
{
public:
	NotifyManager(std::shared_ptr<NotifyConfiguration> configuration);

	bool Add();
	bool Remove(const std::string& targetPath);

	const NotifyDescriptor* Descriptor() const;
	const NotifyConfiguration* Configuration() const;

private:
	std::shared_ptr<NotifyDescriptor> m_descriptor;
	std::shared_ptr<NotifyConfiguration> m_configuration;

	std::unordered_map<std::string /*File path*/, std::shared_ptr<NotifyWatch> /*Watch*/> m_pathToWatchMap;
	std::vector<std::shared_ptr<NotifyWatch>> m_watches;
};

}
