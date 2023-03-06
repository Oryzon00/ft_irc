#pragma once

# include <iostream>
# include <exception>
# include <string>
# include <cctype>
# include <cstdlib>

/* 
Everything in C++ 98.
socket, close, setsockopt, getsockname,
getprotobyname, gethostbyname, getaddrinfo,
freeaddrinfo, bind, connect, listen, accept,
htons, htonl, ntohs, ntohl, inet_addr, inet_ntoa,
send, recv, signal, lseek, fstat, fcntl, poll (or
equivalent) 
*/

/*
 0 - 1023 – the Well Known Ports, also referred to as System Ports.
1024 - 49151 – the Registered Ports, also known as User Ports.
49152 - 65535 – the Dynamic Ports, also referred to as the Private Ports. 
*/


# define SUCCESS 1
# define FAILURE 0
# define ERROR_EXCEPTION 1

struct s_arg
{
	size_t		port;
	std::string	password;
};

typedef struct s_arg	t_arg;

class ParsingException : public std::exception
{
	public:

		virtual const char* what(void) const throw()
		{
			return ("Error: program arguments syntax: ./ircserv <port number> <password>");
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

void	parsing(int ac, char **av, t_arg & arg);
