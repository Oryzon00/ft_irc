#include "irc.hpp"



int	main(int ac, char **av)
{
	try
	{
		t_arg	arg;
		parsing(ac, av, arg);

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
