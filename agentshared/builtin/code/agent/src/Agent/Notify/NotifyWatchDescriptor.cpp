#include "NotifyWatchDescriptor.h"

namespace Agent::Notify
{

NotifyWatchDescriptor::NotifyWatchDescriptor() : m_descriptor(-1)
{
}

NotifyWatchDescriptor::NotifyWatchDescriptor(const NotifyWatchDescriptor& other)
{
	*this = other;
}

NotifyWatchDescriptor::NotifyWatchDescriptor(const int descriptor) : m_descriptor{ descriptor }
{
}

bool NotifyWatchDescriptor::IsValid() const
{
	return m_descriptor != -1;
}

int NotifyWatchDescriptor::Value() const
{
	return m_descriptor;
}

NotifyWatchDescriptor& NotifyWatchDescriptor::operator=(const NotifyWatchDescriptor& rhs)
{
	this->m_descriptor = rhs.Value();
	return *this;
}

bool NotifyWatchDescriptor::operator==(const NotifyWatchDescriptor& rhs) const
{
	return Value() == rhs.Value();
}

int NotifyWatchDescriptor::operator()()
{
	return Value();
}

int NotifyWatchDescriptor::operator()() const
{
	return Value();
}

}
