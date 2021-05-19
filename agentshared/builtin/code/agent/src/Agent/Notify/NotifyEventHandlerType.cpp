#include "LanguageType.h"

#include <algorithm>

namespace Agent::Notify
{

LanguageType LanguageTypeFromString(std::string language)
{
	std::transform(language.begin(), language.end(), language.begin(),
		[](unsigned char letter) { return std::tolower(letter); });

	if (language == "c")
		return LanguageType::C;
	if (language == "cpp")
		return LanguageType::Cpp;
	if (language == "csharp")
		return LanguageType::CSharp;
	if (language == "java")
		return LanguageType::Java;
	if (language == "python")
		return LanguageType::Python;

	return LanguageType::Unknown;
}

LanguageType LanguageTypeFromExtension(std::string extension)
{
	std::transform(extension.begin(), extension.end(), extension.begin(),
		[](unsigned char letter) { return std::tolower(letter); });
	if (extension[0] == '.')
	{
		extension.erase(0, 1);
	}

	if (extension == "c")
		return LanguageType::C;
	if (extension == "cpp")
		return LanguageType::Cpp;
	if (extension == "cs")
		return LanguageType::CSharp;
	if (extension == "java")
		return LanguageType::Java;
	if (extension == "py")
		return LanguageType::Python;

	return LanguageType::Unknown;
}

std::string LanguageTypeToString(LanguageType type)
{
	if (type == LanguageType::C)
		return "c";
	if (type == LanguageType::Cpp)
		return "cpp";
	if (type == LanguageType::CSharp)
		return "csharp";
	if (type == LanguageType::Java)
		return "java";
	if (type == LanguageType::Python)
		return "python";

	return "unknown";
}

}
