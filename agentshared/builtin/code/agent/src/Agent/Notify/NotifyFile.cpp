#include "NotifyFile.h"

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

namespace Agent::Notify
{

/*static*/ bool NotifyFile::FileExists(const std::string& filePath)
{
	return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
}

/*static*/ std::string NotifyFile::ReadFile(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Could not open file " << filePath << std::endl;
		return std::string();
	}

	std::string contents;
	file.seekg(0, std::ios::end);
	contents.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	contents.assign((std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>());

	std::cout << "File " << filePath << " loaded" << std::endl;
	return contents;
}

/*static*/ bool NotifyFile::WriteFile(const std::string& filePath, const std::string& content)
{
	std::filesystem::path path = filePath;
	std::filesystem::path directory = std::filesystem::absolute(path).parent_path();

	if (!std::filesystem::exists(directory))
	{
		auto result = CreateDirectory(directory);
		if (!result)
		{
			std::cerr << "Could not create directory " << directory << std::endl;
			return false;
		}
	}

	std::ofstream outputFile(path, std::ios::out | std::ios::binary);
	if (!outputFile.is_open())
	{
		std::cerr << "File " << filePath << " could not be opened." << std::endl;
		return false;
	}

	outputFile << content;
	return true;
}

/*static*/ bool NotifyFile::DeleteFile(const std::string& filePath)
{
	auto result = std::filesystem::remove(filePath);
	if (!result)
	{
		std::cerr << "Could not delete file " << filePath << std::endl;
		return false;
	}

	return true;
}

/*static*/ bool NotifyFile::CopyFile(const std::string& fromFilePath, const std::string& toFilePath)
{
	auto result = std::filesystem::copy_file(fromFilePath, toFilePath, std::filesystem::copy_options::overwrite_existing);
	if (!result)
	{
		std::cerr << "Could not copy file from " << fromFilePath << " to " << toFilePath << std::endl;
	}
	
	return result;
}

/*static*/ bool NotifyFile::CopyDirectory(const std::string& fromDirectoryPath, const std::string& toDirectoryPath)
{
	auto result = true;
	for (const auto& entry : std::filesystem::directory_iterator(fromDirectoryPath))
	{
		if (!entry.is_regular_file())
		{
			std::cerr << "Ignoring " << entry << " while copying " << fromDirectoryPath << std::endl;
		}
		else
		{
			std::filesystem::path destinationPath = toDirectoryPath + std::filesystem::path::preferred_separator + entry.path().filename().c_str();
			result &= CopyFile(entry.path(), destinationPath);
		}
	}

	return result;
}

/*static*/ bool NotifyFile::CreateDirectory(const std::string& directoryPath)
{
	auto result = std::filesystem::create_directories(directoryPath);
	if (!result)
	{
		std::cerr << "Could not create directory " << directoryPath << std::endl;
	}

	return result;
}

/*static*/ bool NotifyFile::DeleteDirectory(const std::string& directoryPath)
{
	auto result = std::filesystem::remove_all(directoryPath);
	if (!result)
	{
		std::cerr << "Could not delete directory " << directoryPath << std::endl;
	}

	return result;
}

}
