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

void	parsing(int ac, char **av, t_arg & arg)
{
	check_nb_args(ac);
	check_port(av[1]);
	init_arg(av, arg);
	std::cout << "port = " << arg.port << " | password = " << arg.password << std::endl;
}
