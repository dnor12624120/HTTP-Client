#pragma once

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class ConsoleCommand
{
	using CommandFunction = std::function<void(const std::vector<std::string>&)>;

	public:
		ConsoleCommand(const std::string& name, const CommandFunction& function) :
			name{ name },
			function{ function }
		{

		}

		void operator()(const std::vector<std::string>& arguments)
		{
			function(arguments);
		}
	private:
	    std::string name;
		CommandFunction function;
};