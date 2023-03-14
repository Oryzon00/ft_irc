#include "irc.hpp"

void	signal_handler_main_process(int signum)
{
	if (signum == SIGINT)
		throw CloseServerException();
}

void	signal_handler_init(void)
{
	signal(SIGINT, signal_handler_main_process);
}
