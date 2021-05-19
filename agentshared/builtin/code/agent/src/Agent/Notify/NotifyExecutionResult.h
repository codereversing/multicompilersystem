#pragma once

#include "cereal/access.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"

namespace Agent::Notify
{

struct NotifyExecutionResult
{
    int m_returnCode;
	std::string m_output;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("return", m_returnCode),
            cereal::make_nvp("output", m_output));
    }
};

}
