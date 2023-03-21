#include "irc.hpp"


int	main(int ac, char **av)
{
	std::cout << ASCII_COMPUTER << std::endl;
	try
	{
		t_arg	arg;
		parsing(ac, av, arg);
		signal_handler_init();
		execLoop(arg);
	}

	catch(const RestartException & e)
	{
		std::cout << e.what() << std::endl;
		main(ac, av);
	}
	catch (const CloseServerException & e)
	{
		std::cout << e.what() << std::endl;
		return (SUCCESS);
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return (ERROR_EXCEPTION);
	}
	
	return (SUCCESS);
}
