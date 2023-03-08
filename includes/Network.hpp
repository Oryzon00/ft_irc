#pragma once

# include <string>
# include <sys/socket.h>
# include <poll.h>
# include <strings.h>
# include "CustomException.hpp"



class Network
{
	private:
		struct pollfd	_pfds[SOMAXCONN];
		size_t			_nb;

	public:
		Network();

		void	addSocket(int socket);

		void	_poll(void);

		size_t	size(void) const;

		struct pollfd&	operator[](size_t index);

};
