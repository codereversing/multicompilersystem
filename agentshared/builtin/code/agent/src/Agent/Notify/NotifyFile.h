#pragma once

#include <string>

namespace Agent::Notify
{

class NotifyFile
{
public:
	static bool FileExists(const std::string& filePath);
	static std::string ReadFile(const std::string& filePath);
	static bool WriteFile(const std::string& filePath, const std::string& content);
	static bool DeleteFile(const std::string& filePath);
	static bool CopyFile(const std::string& fromFilePath, const std::string& toFilePath);

	static bool CopyDirectory(const std::string& fromDirectoryPath, const std::string& toDirectoryPath);
	static bool CreateDirectory(const std::string& directoryPath);
	static bool DeleteDirectory(const std::string& directoryPath);
};

}
