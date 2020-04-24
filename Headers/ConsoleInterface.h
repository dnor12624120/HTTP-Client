#pragma once

#include <map>
#include <memory>
#include <vector>

#include "AppService.h"
#include "ConsoleCommand.h"

class ConsoleInterface
{
	public:
	    ConsoleInterface(std::unique_ptr<AppService> service);

		void run();
	private:
	    void initializeCommandDict();

		std::string getUserInput();
		std::vector<std::string> parseUserInput(const std::string& input);
		void executeCommand(const std::vector<std::string>& arguments);
		bool findCommand(std::string command);

		std::vector<std::string> isFlagPresent(const std::vector<std::string>& arguments, const std::string& flag, int numArgsToReturn);

		void exit(const std::vector<std::string>& arguments);
		void resolveDomain(const std::vector<std::string>& arguments);
		void sendGETRequest(const std::vector<std::string>& arguments);

	    bool running;
		std::map<std::string, std::unique_ptr<ConsoleCommand>> commands;
		std::unique_ptr<AppService> service;
};