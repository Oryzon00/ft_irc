#include "irc.hpp"

void	signal_handler_main_process(int signum)
{
	if (signum == SIGINT)
		throw CloseServerException();
	if (signum == SIGQUIT)
		std::cout << "SIGQUIT detected" << std::endl;
}

void	signal_handler_init(void)
{
	signal(SIGINT, signal_handler_main_process);
	signal(SIGQUIT, signal_handler_main_process);
}
