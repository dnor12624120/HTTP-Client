#include <boost/asio.hpp>

#include <iostream>
#include <functional>
#include <string>

#include "AppService.h"
#include "ConsoleInterface.h"
#include "HttpService.h"

using namespace boost::asio;

int main(int argc, char** argv)
{
	io_service service;

	std::unique_ptr<HttpService> httpService(new HttpService(service));

	std::unique_ptr<AppService> appService(new AppService(std::move(httpService)));

	ConsoleInterface consoleInterface(std::move(appService));
	consoleInterface.run();
	return 0;
}

