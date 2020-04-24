#pragma once

#include <exception>
#include <string>

class ConsoleInterfaceException : public std::exception
{
	public:
	    explicit ConsoleInterfaceException(const char* message) :
			message{ message }
	    {

	    }

	
	    explicit ConsoleInterfaceException(const std::string& message) :
			message{ message }
	    {

		}

	
	    virtual const char* what() const throw ()
		{
		    return message.c_str();
	    }

	protected:
	    std::string message;
};

class HttpClientException : public std::exception
{
	public:
		explicit HttpClientException(const char* message) :
			message{ message }
		{

		}


		explicit HttpClientException(const std::string& message) :
			message{ message }
		{

		}


		virtual const char* what() const throw ()
		{
			return message.c_str();
		}

	protected:
	    std::string message;
};