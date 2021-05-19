#pragma once

#include <string>

#include "NotifyEvent.h"

namespace Agent::Notify
{

class NotifyChildProcess
{
public:
	NotifyChildProcess(std::shared_ptr<NotifyEvent> notifyEvent);
	~NotifyChildProcess();

	bool Launch();

	int Result() const;
	bool Success() const;
	bool Failure() const;

	const std::string& Output() const;

private:
	void buildCommand(std::shared_ptr<NotifyEvent> notifyEvent);

	const std::string OutputFilePath() const;
	const std::string GetBuiltOutputPathBase() const;

	int m_result;
	std::string m_output;

	std::shared_ptr<NotifyEvent> m_event;

	std::string m_builtCommand;
};

}
