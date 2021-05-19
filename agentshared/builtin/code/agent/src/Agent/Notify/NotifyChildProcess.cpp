#include "NotifyChildProcess.h"

#include <array>
#include <cstdio>
#include <filesystem>
#include <iostream>

#include "NotifyFile.h"

namespace Agent::Notify
{

NotifyChildProcess::NotifyChildProcess(std::shared_ptr<NotifyEvent> notifyEvent)
{
	buildCommand(notifyEvent);
}

NotifyChildProcess::~NotifyChildProcess()
{
	NotifyFile::DeleteFile(OutputFilePath());
}

void NotifyChildProcess::buildCommand(std::shared_ptr<NotifyEvent> notifyEvent)
{
	const auto argumentsFullPath = notifyEvent->ArgumentsPath() + std::filesystem::path::preferred_separator + notifyEvent->FileName() + ".args";
	const auto inputFileFullPath = notifyEvent->InputPath() + std::filesystem::path::preferred_separator + notifyEvent->FileName();
	const auto stdinFileFullPath = notifyEvent->StdinPath() + std::filesystem::path::preferred_separator + notifyEvent->FileName() + ".stdin";

	m_builtCommand.reserve(512);
	m_builtCommand = notifyEvent->BootstrapPath() + std::filesystem::path::preferred_separator + notifyEvent->BootstrapScriptName()
		+ " -f " + inputFileFullPath
		+ " -a " + argumentsFullPath
		+ " -s " + stdinFileFullPath
		+ " -t " + notifyEvent->InteractiveTimeout()
		+ " -i " + std::to_string(notifyEvent->Index())
		+ " -d " + notifyEvent->DependenciesPath()
		+ " -w " + notifyEvent->WorkspacePath()
		+ " -o " + notifyEvent->OutputPath()
		+ " -l " + notifyEvent->Language();
}

bool NotifyChildProcess::Launch()
{
	auto pipe = popen(m_builtCommand.c_str(), "r");
	if (!pipe)
	{
		perror("popen");
		std::cerr << "Could not execute " << m_builtCommand << std::endl;
		return false;
	}

	m_result = pclose(pipe);

	m_output = NotifyFile::ReadFile(OutputFilePath());

	return Success();
}

int NotifyChildProcess::Result() const
{
	return m_result;
}

bool NotifyChildProcess::Success() const
{
	return m_result == EXIT_SUCCESS;
}

bool NotifyChildProcess::Failure() const
{
	return m_result == EXIT_FAILURE;
}

const std::string& NotifyChildProcess::Output() const
{
	return m_output;
}

const std::string NotifyChildProcess::OutputFilePath() const
{
	auto basePath = GetBuiltOutputPathBase();
	return basePath + "-output.log";
}

const std::string NotifyChildProcess::GetBuiltOutputPathBase() const
{
	return m_event->OutputPath() + std::filesystem::path::preferred_separator
		+ std::to_string(m_event->Index()) + std::filesystem::path::preferred_separator + m_event->FileName();
}

}
