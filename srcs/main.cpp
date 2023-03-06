#include "ft_irc.hpp"

int	main(int ac, char **av)
{
	try
	{
		t_arg	arg;
		parsing(ac, av, arg);
		std::cout << "parsing OK" << std::endl;
		std::cout << "port = " << arg.port << " | password = " << arg.password << std::endl;
	}

	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return (ERROR_EXCEPTION);
	}
	
	return (0);
}
