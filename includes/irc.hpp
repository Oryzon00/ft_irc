#pragma once

# include <iostream>
# include <string>
# include <cctype>
# include <cstdlib>
# include <signal.h>

# include "CustomException.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "parsing.hpp"

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

void	execLoop(Server &server);

# define SUCCESS 			1
# define FAILURE 			0
# define ERROR_EXCEPTION	1
# define SERVER_INDEX		0
# define DISCONNECT			0
