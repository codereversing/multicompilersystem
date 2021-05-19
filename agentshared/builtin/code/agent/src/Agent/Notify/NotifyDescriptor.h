#pragma once

#include "NotifyWatchDescriptor.h"

namespace Agent::Notify
{

class NotifyDescriptor : public NotifyWatchDescriptor
{
public:
	NotifyDescriptor();
	NotifyDescriptor(const int descriptor);
	virtual ~NotifyDescriptor();
};

}
