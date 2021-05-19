#include "NotifyWatch.h"

#include <cstdint>
#include <iostream>

#include <sys/inotify.h>

namespace Agent::Notify
{

NotifyWatch::NotifyWatch(std::shared_ptr<NotifyDescriptor> notifyDescriptor, std::shared_ptr<NotifyConfiguration> configuration)
{
	m_notifyDescriptor = notifyDescriptor;
	m_watchDescriptor = std::make_shared<NotifyWatchDescriptor>();
	m_targetPath = configuration->m_inputPath;
}

NotifyWatch::~NotifyWatch()
{
	Remove();
}

bool NotifyWatch::Add()
{
	if (IsActive()) {
		std::cerr << "Cannot add watch because it is already active" << std::endl;
		return false;
	}

	auto result = inotify_add_watch(m_notifyDescriptor->Value(), m_targetPath.c_str(), IN_CLOSE_WRITE);
	if (result == -1) {
		perror("inotify_add_watch");
		return false;
	}

	std::cout << "Setting new watch descriptor to " << result << std::endl;
	m_watchDescriptor.reset(new NotifyWatchDescriptor(result));

	auto isActive = IsActive();
	std::cout << "Watch descriptor " << m_watchDescriptor->Value() << " is active? " << std::boolalpha << isActive << std::endl;

	return isActive;
}

bool NotifyWatch::Remove()
{
	if (!IsActive()) {
		std::cerr << "Cannot remove watch because it is not active" << std::endl;
		return false;
	}

	auto result = inotify_rm_watch(m_notifyDescriptor->Value(), m_watchDescriptor->Value());
	if (result == -1) {
		perror("inotify_rm_watch");
		return false;
	}

	std::cout << "Watch descriptor for " << m_watchDescriptor->Value() << " removed successfully." << std::endl;
	m_watchDescriptor.reset(new NotifyWatchDescriptor(-1));

	auto isActive = IsActive();
	std::cout << "Watch descriptor " << m_watchDescriptor->Value() << " is active? " << std::boolalpha << isActive << std::endl;

	return isActive;
}

bool NotifyWatch::IsActive() const
{
	return m_notifyDescriptor->IsValid() && m_watchDescriptor->IsValid();
}

}
