#pragma once


# include "CustomException.hpp"
# include "Server.hpp"
# include "parsing.hpp"
# include "signal.hpp"


void	execLoop(t_arg &arg);

# define SUCCESS			0
# define ERROR_EXCEPTION	1
# define SERVER_INDEX		0
