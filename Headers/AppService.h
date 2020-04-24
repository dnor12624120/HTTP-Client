#pragma once

#include <memory>

#include "HttpService.h"

class AppService
{
	public:
	    AppService(std::unique_ptr<HttpService> httpService);

		std::vector<ip::tcp::endpoint> resolveDomain(const std::string& domain);
		std::string convertGetRequestToString(const std::string& domain, bool headerFlag);
	private:
	    std::unique_ptr<HttpService> httpService;
};