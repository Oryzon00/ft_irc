#include "irc.hpp"
#include <limits.h>

void	check_nb_args(int ac)
{
	if (ac != 3)
		throw ParsingException();
}
void	check_port(char *port)
{
	int i = 0;
	while (port[i])
	{
		if (!isdigit(port[i]))
			throw PortDigitException();
		i++;
	}
	int port_nb = atoi(port);
	if (port_nb > USHRT_MAX)
		throw PortNumberException();

}

void	init_arg(char **av, t_arg & arg)
{
	arg.port = atoi(av[1]);
	arg.password = std::string(av[2]);
}

void	print_arg(t_arg& arg)
{
	std::cout << "server is reading on port " << arg.port << std::endl;
	std::cout << "server password is '" << arg.password << "'" <<  std::endl;
	std::cout << std::endl;
}

void	parsing(int ac, char **av, t_arg & arg)
{
	check_nb_args(ac);
	check_port(av[1]);
	init_arg(av, arg);
	print_arg(arg);
	
}
