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


void	execLoop(Server &server);
void	sigint_handler_main_process(int signum);

# define SUCCESS 			1
# define FAILURE 			0
# define ERROR_EXCEPTION	1
# define SERVER_INDEX		0
# define DISCONNECT			0
