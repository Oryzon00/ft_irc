#pragma once

# include <string>
# include <sys/socket.h>
# include <poll.h>
# include <strings.h>



class Network
{
	private:
		struct pollfd	_pfds[SOMAXCONN];
		size_t			_nb;

	public:
		Network();

		void	addSocket(int socket);

		const struct
};
