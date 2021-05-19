#include "NotifyEvent.h"

#include <filesystem>

#include "NotifyFile.h"

namespace Agent::Notify
{

/*static*/ std::atomic_int32_t NotifyEvent::m_folderNumber = 0;

NotifyEvent::NotifyEvent(const NotifyConfiguration* config, const std::string& fileName, int watchDescriptor, int mask)
{
	std::filesystem::path inputFilePath = fileName;
	m_fileName = inputFilePath.filename();
	m_language = inputFilePath.extension();
	if (m_language.length() > 0 && m_language[0] == '.')
	{
		m_language.erase(0, 1);
	}

	m_index = m_folderNumber++;
	std::string folderRelativePath = std::filesystem::path::preferred_separator + std::to_string(m_index);

	m_inputPath = config->m_inputPath;
	m_outputPath = config->m_outputPath;
	m_workspacePath = config->m_workspacePath;
	m_argumentsPath = config->m_argumentsPath;
	m_stdinPath = config->m_stdinPath;
	m_dependenciesPath = config->m_dependenciesPath + std::filesystem::path::preferred_separator + m_language;
	m_bootstrapPath = config->m_bootstrapPath;
	m_bootstrapScriptName = config->m_bootstrapScriptName;
	m_interactiveTimeout = config->m_interactiveTimeout;
	m_watchDescriptor = watchDescriptor;
	m_mask = mask;
}

const std::string& NotifyEvent::FileName() const
{
	return m_fileName;
}

const std::string& NotifyEvent::InputPath() const
{
	return m_inputPath;
}

const std::string& NotifyEvent::OutputPath() const
{
	return m_outputPath;
}

const std::string& NotifyEvent::WorkspacePath() const
{
	return m_workspacePath;
}

const std::string& NotifyEvent::DependenciesPath() const
{
	return m_dependenciesPath;
}

const std::string& NotifyEvent::ArgumentsPath() const
{
	return m_argumentsPath;
}

const std::string& NotifyEvent::StdinPath() const
{
	return m_stdinPath;
}

const std::string& NotifyEvent::InteractiveTimeout() const
{
	return m_interactiveTimeout;
}

const std::string& NotifyEvent::BootstrapPath() const
{
	return m_bootstrapPath;
}

const std::string& NotifyEvent::BootstrapScriptName() const
{
	return m_bootstrapScriptName;
}

int NotifyEvent::Index() const
{
	return m_index;
}

int NotifyEvent::WatchDescriptor() const
{
	return m_watchDescriptor;
}

int NotifyEvent::Mask() const
{
	return m_mask;
}

const std::string& NotifyEvent::Language() const
{
	return m_language;
}

bool NotifyEvent::IsValid() const
{
	return !m_fileName.empty() && !m_outputPath.empty() && !m_inputPath.empty()
		&& !m_workspacePath.empty() && !m_dependenciesPath.empty()
		&& !m_bootstrapPath.empty() && (m_watchDescriptor != -1)
		&& m_language.empty();
}


}
