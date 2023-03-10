#include "irc.hpp"

void	sigint_handler_main_process(int signum)
{
	(void) signum;
	throw CloseServerException();
}
