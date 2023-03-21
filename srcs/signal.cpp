# include "signal.hpp"
# include "CustomException.hpp"

void	signal_handler_main_process(int signum)
{
	if (signum == SIGINT)
		throw CloseServerException();
	else if (signum == SIGPIPE)
		return ;
}

void	signal_handler_init(void)
{
	signal(SIGINT, signal_handler_main_process);
	signal(SIGPIPE, signal_handler_main_process);
}
