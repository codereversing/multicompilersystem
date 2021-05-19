#include <iostream>
#include <memory>

#include <csignal>
#include <unistd.h>

#include "Agent/Notify/NotifyConfiguration.h"
#include "Agent/Notify/NotifyEventDispatcher.h"
#include "Agent/Notify/NotifyEventHandler.h"
#include "Agent/Notify/NotifyEventHandlerLanguage.h"
#include "Agent/Notify/NotifyEventHandlerTopmost.h"
#include "Agent/Notify/NotifyFile.h"
#include "Agent/Notify/NotifyManager.h"
#include "Agent/Notify/NotifySerializer.h"

std::shared_ptr<Agent::Notify::NotifyConfiguration> ReadConfiguration(const std::string& filePath)
{
	auto config = Agent::Notify::NotifySerializer<Agent::Notify::NotifyConfiguration>::Read(filePath, "configuration");
	if (config == nullptr || !config->IsValid())
	{
		std::cerr << "Cannot launch with invalid configuration. Exiting." << std::endl;
		exit(-1);
	}

	return config;
}

std::unique_ptr<Agent::Notify::NotifyEventHandler> BuildHandlers(std::shared_ptr<Agent::Notify::NotifyConfiguration> configuration)
{
	std::unique_ptr<Agent::Notify::NotifyEventHandlerTopmost> topLevelHandler(new Agent::Notify::NotifyEventHandlerTopmost());

	for(auto const language : configuration->m_supportedLanguages)
	{
		std::cout << "Adding handler for " << language << std::endl;
		topLevelHandler->AddHandler(std::make_unique<Agent::Notify::NotifyEventHandlerLanguage>(language));
		break;
	}

	return topLevelHandler;
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		exit(-1);
	}

	auto config = ReadConfiguration(argv[1]);
	auto handler = BuildHandlers(config);
	auto manager = std::make_unique<Agent::Notify::NotifyManager>(config);

	auto result = manager->Add();
	if (!result)
	{
		std::cerr << "Could not add watch." << std::endl;
		exit(-1);
	}

	auto dispatcher = std::make_unique<Agent::Notify::NotifyEventDispatcher>(std::move(manager), std::move(handler));
	dispatcher->BeginEventLoop();

	return 0;
}
