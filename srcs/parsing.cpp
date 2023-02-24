#include "ft_irc.hpp"

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
			throw ParsingException();
		i++;
	}
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
}
