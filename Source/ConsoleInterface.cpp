#include "ConsoleInterface.h"

#include <algorithm>
#include <iostream>
#include <sstream>

#include "Exception.h"

ConsoleInterface::ConsoleInterface(std::unique_ptr<AppService> service) :
	running{ false },
	service { std::move(service) }
{
	initializeCommandDict();
}

void ConsoleInterface::run()
{
	running = true;
	while (running)
	{
		std::string input = getUserInput();
		std::vector<std::string> argumentList = parseUserInput(input);
		executeCommand(argumentList);
	}
}

std::string ConsoleInterface::getUserInput()
{
	std::string input;
	std::cout << ">";
	std::getline(std::cin, input);
	return input;
}

std::vector<std::string> ConsoleInterface::parseUserInput(const std::string& input)
{
	std::string argument;
	std::vector<std::string> argumentList;
	std::stringstream stringstreamInput(input);

	while (std::getline(stringstreamInput, argument, ' '))
	{
		argumentList.emplace_back(argument);
	}

	return argumentList;
}

void ConsoleInterface::executeCommand(const std::vector<std::string>& argumentList)
{
	std::string command = argumentList[0];
	std::vector<std::string> arguments = std::vector<std::string>(argumentList.begin() + 1, argumentList.end());
	if (!findCommand(command))
	{
		throw ConsoleInterfaceException("Command " + command + " does not exist.");
	}
	(*commands[command])(arguments);
}

bool ConsoleInterface::findCommand(std::string command)
{
	return commands.find(command) != commands.end();
}

void ConsoleInterface::initializeCommandDict()
{
	commands["exit"] = std::unique_ptr<ConsoleCommand>(new ConsoleCommand("exit", std::bind(&ConsoleInterface::exit, this, std::placeholders::_1)));
	commands["resolve"] = std::unique_ptr<ConsoleCommand>(new ConsoleCommand("resolve", std::bind(&ConsoleInterface::resolveDomain, this, std::placeholders::_1)));
	commands["get"] = std::unique_ptr<ConsoleCommand>(new ConsoleCommand("get", std::bind(&ConsoleInterface::sendGETRequest, this, std::placeholders::_1)));
}

void ConsoleInterface::exit(const std::vector<std::string>& arguments)
{
	running = false;
}

void ConsoleInterface::resolveDomain(const std::vector<std::string>& arguments)
{
	try
	{
		std::vector<ip::tcp::endpoint> ips = service->resolveDomain(arguments[0]);
		std::cout << arguments[0] << " resolved to\n";
		std::for_each(ips.begin(), ips.end(), [](const auto& ip) { std::cout << ip << '\n'; });
	}
	catch (boost::system::system_error error)
	{
		std::cout << error.what() << '\n';
	}
}

void ConsoleInterface::sendGETRequest(const std::vector<std::string>& arguments)
{
	try
	{ 
		auto flags = isFlagPresent(arguments, "-h", 1);
		std::cout << service->convertGetRequestToString(arguments[0], flags.size() != 0) << '\n';
	}
	catch (boost::system::system_error error)
	{
		std::cout << error.what() << '\n';
	}
}

std::vector<std::string> ConsoleInterface::isFlagPresent(const std::vector<std::string>& arguments, const std::string& flag, int numArgsToReturn)
{
	auto position = std::find(arguments.begin(), arguments.end(), flag);
	if (position != arguments.end())
	{
		return std::vector<std::string>(position, position + numArgsToReturn);
	}
	return std::vector<std::string>(0);
}
