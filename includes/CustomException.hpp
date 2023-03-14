#pragma once

# include <exception>
# include <string>
# include "ASCII.hpp"

class ParsingException : public std::exception
{
	public:

		virtual const char* what(void) const throw()
		{
			return ("Error: program arguments syntax: ./ircserv <port number> <password>");
		}
};

class PortDigitException : public std::exception
{
	public:

		virtual const char* what(void) const throw()
		{
			return ("Error: port number must only be digit (0-9)");
		}
};

class PortNumberException : public std::exception
{
	public:

		virtual const char* what(void) const throw()
		{
			return ("Error: port number must be between 1 and USHRT_MAX (65,535)");
		}
};

class SocketException : public std::exception
{
	std::string	_msg;

	public:
		SocketException(const std::string msg)
			: _msg(std::string("Error: socket error: ") + msg)	{}

		~SocketException() throw()	{}

		virtual const char*	what(void) const throw()
		{
			return _msg.c_str();
		}
};

class CustomException : public std::exception
{
	std::string	_msg;

	public:
		CustomException(const std::string msg)	: _msg(msg)	{}

		~CustomException() throw()	{}

		virtual const char*	what(void) const throw()
		{
			return _msg.c_str();
		}
};

class CloseServerException : public std::exception
{
	public:

		virtual const char* what(void) const throw()
		{
			return (ASCII_PENGUIN);
		}
};
