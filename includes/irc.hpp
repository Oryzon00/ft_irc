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
# include <errno.h>
# include "ASCII.hpp"


void	execLoop(Server &server);
void	signal_handler_init(void);

# define SUCCESS			0
# define ERROR_EXCEPTION	1
# define SERVER_INDEX		0
