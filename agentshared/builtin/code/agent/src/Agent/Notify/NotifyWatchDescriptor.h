#pragma once

namespace Agent::Notify
{

class NotifyWatchDescriptor
{
public:
	NotifyWatchDescriptor();
	NotifyWatchDescriptor(const NotifyWatchDescriptor& other);
	NotifyWatchDescriptor(const int descriptor);

	NotifyWatchDescriptor& operator=(const NotifyWatchDescriptor& rhs);
	bool operator==(const NotifyWatchDescriptor& rhs) const;

	bool IsValid() const;
	int Value() const;

	int operator()();
	int operator()() const;

protected:
	int m_descriptor;
};

}
