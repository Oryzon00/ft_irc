#include "irc.hpp"


void	sigint_handler_main_process(int signum)
{
	(void) signum;
	throw CloseServerException();
}


int	main(int ac, char **av)
{
	try
	{
		t_arg	arg;
		parsing(ac, av, arg);

		signal(SIGINT, sigint_handler_main_process);
		Server	server(arg.port, arg.password);
		execLoop(server);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	}

	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return (ERROR_EXCEPTION);
	}
	
	return (0);
}
