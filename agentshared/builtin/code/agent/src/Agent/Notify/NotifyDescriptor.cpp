#include "NotifyDescriptor.h"

#include <unistd.h>

namespace Agent::Notify
{
NotifyDescriptor::NotifyDescriptor() : NotifyWatchDescriptor()
{
}

NotifyDescriptor::NotifyDescriptor(const int descriptor) : NotifyWatchDescriptor(descriptor)
{
}

NotifyDescriptor::~NotifyDescriptor()
{
	close(Value());
}

}
