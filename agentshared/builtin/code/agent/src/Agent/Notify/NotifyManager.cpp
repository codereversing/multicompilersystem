#include "NotifyManager.h"

#include <algorithm>
#include <iostream>

#include <sys/inotify.h>

namespace Agent::Notify
{

NotifyManager::NotifyManager(std::shared_ptr<NotifyConfiguration> configuration)
{
	m_configuration = configuration;
	auto result = inotify_init();
	m_descriptor = std::make_shared<NotifyDescriptor>();
	if (result == -1)
	{
		perror("inotify_init");
	}
	else
	{
		std::cout << "inotify API initialized successfully. Descriptor = " << result << std::endl;
		m_descriptor.reset(new NotifyDescriptor(result));
	}
}

bool NotifyManager::Add()
{
	if (!m_descriptor->IsValid())
	{
		std::cerr << "Cannot add watch. Descriptor " << m_descriptor->Value() << " is not valid." << std::endl;
		return false;
	}

	if (m_pathToWatchMap.find(m_configuration->m_inputPath) != m_pathToWatchMap.end())
	{
		std::cerr << "Cannot add watch. Watch on " << m_configuration->m_inputPath << " already exists." << std::endl;
		return false;
	}

	std::shared_ptr<NotifyWatch> newWatch(new NotifyWatch(m_descriptor, m_configuration));
	if (newWatch->Add())
	{
		std::cout << "Watch on " << m_configuration->m_inputPath << " added successfully." << std::endl;
		m_pathToWatchMap.insert({ m_configuration->m_inputPath, newWatch });
		m_watches.push_back(newWatch);
		return true;
	}

	std::cerr << "Could not add watch on " << m_configuration->m_inputPath << std::endl;
	return false;
}

bool NotifyManager::Remove(const std::string& targetPath)
{
	if (!m_descriptor->IsValid())
	{
		return false;
	}

	auto watchIter = m_pathToWatchMap.find(targetPath);
	if (watchIter == m_pathToWatchMap.end())
	{
		return false;
	}

	auto watch = watchIter->second;
	auto result = watch->Remove();

	m_watches.erase(std::remove(m_watches.begin(), m_watches.end(), watch), m_watches.end());
	m_pathToWatchMap.erase(targetPath);

	return result;
}

const NotifyDescriptor* NotifyManager::Descriptor() const
{
	return m_descriptor.get();
}

const NotifyConfiguration* NotifyManager::Configuration() const
{
	return m_configuration.get();
}

}
