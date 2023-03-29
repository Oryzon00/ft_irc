#pragma once

# include <string>
# include <cstring>

struct s_arg
{
	size_t		port;
	std::string	password;
};

typedef struct s_arg	t_arg;

void	parsing(int ac, char **av, t_arg & arg);
