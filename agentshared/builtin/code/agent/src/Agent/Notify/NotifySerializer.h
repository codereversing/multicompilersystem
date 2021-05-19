#pragma once

#include <iostream>
#include <fstream>
#include <memory>
#include <string>

namespace Agent::Notify
{

template <typename T>
class NotifySerializer
{
public:
	static std::shared_ptr<T> Read(const std::string& filePath, const std::string& name)
	{
		std::ifstream fileStream(filePath);
		if (!fileStream.is_open())
		{
			std::cerr << "Failed to open " << filePath << std::endl;
			return nullptr;
		}

		T object;
		{
			cereal::JSONInputArchive iarchive(fileStream);
			iarchive(cereal::make_nvp(name.c_str(), object));
		}

		return std::make_shared<T>(object);
	}

	static bool Write(const std::string& filePath, const std::string& name, const T& object)
	{
		std::ofstream fileStream(filePath);
		if (!fileStream.is_open())
		{
			std::cerr << "Could not open " << filePath << std::endl;
			return false;
		}

		{
			cereal::JSONOutputArchive iarchive(fileStream);
			iarchive(cereal::make_nvp(name, object));
		}

		return true;
	}
};

}
