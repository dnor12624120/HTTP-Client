#pragma once

#include <iostream>
#include <regex>
#include <sstream>

#include "Exception.h"
#include "HttpService.h"

HttpService::HttpService(io_service& service):
	service { service },
	hostResolver{ ip::tcp::resolver(service) },
	socket{ ip::tcp::socket(service) },
	stream{ tcp_stream(service) }
{

}

std::vector<ip::tcp::endpoint> HttpService::domainToEndpoint(const std::string& host)
{
	std::vector<ip::tcp::endpoint> endpoints;
	auto results = hostResolver.resolve(host);
	std::for_each(results.begin(), results.end(), [&endpoints](auto& result) { endpoints.emplace_back(result); });
	return endpoints;
}

std::string HttpService::convertGetRequestToString(const std::string& domain)
{
	std::pair<std::string, std::string> split = separateResourceFromDomain(domain);
	std::string resource = split.first;
	std::string  host = split.second;
	connectToHost(host);
	sendGetRequest(host, resource);
	return readResponse();
}

std::string HttpService::convertGetRequestToStringWithHeader(const std::string& domain)
{
	std::pair<std::string, std::string> split = separateResourceFromDomain(domain);
	std::string resource = split.first;
	std::string  host = split.second;
	connectToHost(host);
	sendGetRequest(host, resource);
	return readResponseWithHeader();
}

void HttpService::connectToHost(const std::string& host)
{
	auto endpoints = hostResolver.resolve(host, "80");
	stream.connect(endpoints);
}

void HttpService::sendGetRequest(const std::string& host, const std::string& resource)
{
	http::request<http::string_body> request(http::verb::get, resource, 11);
	request.set(http::field::host, host);
	http::write(stream, request);
}

std::string HttpService::readResponse()
{
	flat_buffer buffer;
	http::response<http::dynamic_body> response;
	http::read(stream, buffer, response);
	return boost::beast::buffers_to_string(response.body().data());
}

std::string HttpService::readResponseWithHeader()
{
	flat_buffer buffer;
	http::response<http::dynamic_body> response;
	http::read(stream, buffer, response);
	std::stringstream stringstream;
	stringstream << response << '\n';
	return stringstream.str();
}


std::pair<std::string, std::string> HttpService::separateResourceFromDomain(const std::string& domain)
{
	std::regex uriRegex("(?:[a-z0-9](?:[a-z0-9-]{0,61}[a-z0-9])?\\.)+[a-z0-9][a-z0-9-]{0,61}[a-z0-9]");
	std::smatch match;
	std::regex_search(domain, match, uriRegex);
	if (match.size() == 0)
	{
		throw HttpClientException("Could not identify host & resource in domain.");
	}
	else
	{
		int resourceStringStartIndex = domain.find(match.str());
		return std::pair<std::string, std::string>(
			std::string(domain.begin() + resourceStringStartIndex + match.length(), domain.end()),
			match.str());
	}
}
