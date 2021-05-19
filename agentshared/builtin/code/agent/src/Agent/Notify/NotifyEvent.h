#pragma once

#include <atomic>
#include <string>

#include "NotifyConfiguration.h"

namespace Agent::Notify
{

class NotifyEvent
{
public: 
	NotifyEvent(const NotifyConfiguration* config, const std::string& fileName, const int watchDescriptor, int mask);

	const std::string& FileName() const;
	const std::string& InputPath() const;
	const std::string& OutputPath() const;
	const std::string& WorkspacePath() const;
	const std::string& DependenciesPath() const;
	const std::string& ArgumentsPath() const;
	const std::string& StdinPath() const;
	const std::string& InteractiveTimeout() const;
	const std::string& BootstrapPath() const;
	const std::string& BootstrapScriptName() const;

	int Index() const;
	int WatchDescriptor() const;
	int Mask() const;
	const std::string& Language() const;

	bool IsValid() const;

private:
	std::string m_fileName;
	std::string m_outputPath;
	std::string m_inputPath;
	std::string m_workspacePath;
	std::string m_dependenciesPath;
	std::string m_argumentsPath;
	std::string m_stdinPath;
	std::string m_interactiveTimeout;
	std::string m_bootstrapPath;
	std::string m_bootstrapScriptName;

	int m_index;
	int m_watchDescriptor;
	int m_mask;
	std::string m_language;

	static std::atomic_int32_t m_folderNumber;
};

}