#include "NotifyEventHandlerLanguage.h"

#include <filesystem>
#include <iostream>

#include "NotifyChildProcess.h"
#include "NotifyExecutionResult.h"
#include "NotifyFile.h"
#include "NotifySerializer.h"

namespace Agent::Notify
{

NotifyEventHandlerLanguage::NotifyEventHandlerLanguage(const std::string& language) : NotifyEventHandler(language)
{
}

void NotifyEventHandlerLanguage::Handle(std::shared_ptr<NotifyEvent> event)
{
	NotifyChildProcess childProcess(event);
	childProcess.Launch();

	auto outputPath = event->OutputPath() + std::filesystem::path::preferred_separator + std::to_string(event->Index()) +
		std::filesystem::path::preferred_separator + event->FileName() + ".log";
	const NotifyExecutionResult resultObject{ childProcess.Result(), childProcess.Output() };
	NotifySerializer<NotifyExecutionResult>::Write(outputPath, "result", resultObject);
}

}
