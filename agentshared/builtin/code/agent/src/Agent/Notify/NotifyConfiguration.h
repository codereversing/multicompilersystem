#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "cereal/access.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

namespace Agent::Notify
{

struct NotifyConfiguration
{
	std::string m_inputPath;
    std::string m_outputPath;
    std::string m_workspacePath;
    std::string m_dependenciesPath;
    std::string m_argumentsPath;
    std::string m_stdinPath;
    std::string m_interactiveTimeout;
    std::string m_relocatePath;
    std::string m_bootstrapPath;
    std::string m_bootstrapScriptName;
    std::vector<std::string> m_supportedLanguages;
    bool m_isMultithreaded;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("inputpath", m_inputPath),
            cereal::make_nvp("outputpath", m_outputPath),
            cereal::make_nvp("workspacepath", m_workspacePath),
            cereal::make_nvp("dependenciespath", m_dependenciesPath),
            cereal::make_nvp("argumentspath", m_argumentsPath),
            cereal::make_nvp("stdinpath", m_stdinPath),
            cereal::make_nvp("interactivetimeout", m_interactiveTimeout),
            cereal::make_nvp("relocatepath", m_relocatePath),
            cereal::make_nvp("bootstrappath", m_bootstrapPath),
            cereal::make_nvp("bootstrapscriptname", m_bootstrapScriptName),
            cereal::make_nvp("supportedlanguages", m_supportedLanguages),
            cereal::make_nvp("ismultithreaded", m_isMultithreaded));
    }

    bool IsValid() const
    {
        return !m_inputPath.empty() && !m_outputPath.empty() && !m_workspacePath.empty()
            && !m_dependenciesPath.empty() && !m_relocatePath.empty() && !m_bootstrapPath.empty() 
            && !m_bootstrapScriptName.empty() && !m_stdinPath.empty() && !m_interactiveTimeout.empty();
    }
};

}
