#include "AppService.h"

AppService::AppService(std::unique_ptr<HttpService> httpService) :
	httpService{ std::move(httpService) }
{

}

std::vector<ip::tcp::endpoint> AppService::resolveDomain(const std::string& domain)
{
	return httpService->domainToEndpoint(domain);
}

std::string AppService::convertGetRequestToString(const std::string& domain, bool headerFlag)
{
	return headerFlag ? httpService->convertGetRequestToStringWithHeader(domain) :
	                    httpService->convertGetRequestToString(domain);
}