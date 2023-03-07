#include "irc.hpp"
#include <poll.h>
# include <sys/socket.h>
#include <unistd.h>
#include <strings.h>

// POLLIN	Alert me when data is ready to recv() on this socket.
// POLLOUT	Alert me when I can send() data to this socket without blocking.

// struct pollfd {
//     int fd;         // the socket descriptor
//     short events;   // bitmap of events we're interested in
//     short revents;  // when poll() returns, bitmap of events that occurred
// };

int	initClientSocket(int socket_server);

void	execLoop(Server &server)
{
	char	buffer[512] = {0};

	while (42)
	{
		poll(server.pfds, nb, 0); //serverpoll
		for(size_t i = 0; i < nb; i++)
		{
			if (pfds[i].revents & POLLIN) //checkSocket
			{
				std::cout << "fd nb " << i << " ready" << std::endl;
				if (i == SERVER_INDEX)//if server is ready --> new connection
				{
					//add client
					std::cout << "new connection to server" << std::endl;
					pfds[nb].fd = initClientSocket(server.getServerSocket());
					pfds[nb].events = POLLIN;
					nb++;
				}
				else //client is ready --> new connection
				{
					//printMsg
					size_t	ret = recv(pfds[i].fd, buffer, sizeof(buffer), 0);
					std::cout << "Message received from client: " << std::endl << buffer << std::endl;

					(void)ret;
				}
			}
		}
	}


}
