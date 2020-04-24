#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <memory>
#include <string>
#include <utility>

using namespace boost::asio;
using namespace boost::beast;
using namespace boost::beast::http;

class HttpService
{
	public:
	    HttpService(io_service& service);

		std::vector<ip::tcp::endpoint> domainToEndpoint(const std::string& domain);
		std::string convertGetRequestToString(const std::string& domain);
		std::string convertGetRequestToStringWithHeader(const std::string& domain);

	private:
	    void connectToHost(const std::string& host);
	    void sendGetRequest(const std::string& host, const std::string& resource);
		std::string readResponse();
		std::string readResponseWithHeader();

		std::pair<std::string, std::string> separateResourceFromDomain(const std::string& domain);

	    io_service& service;
		ip::tcp::resolver hostResolver;
		ip::tcp::socket socket;
		tcp_stream stream;
		std::string body;
};