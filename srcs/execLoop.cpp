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
		server.poll();
		for(size_t i = 0; i < server.getNetwork().size(); i++)
		{
			if (server.checkSocket(i, POLLIN)) //checkSocket
			{
				std::cout << "fd nb " << i << " ready" << std::endl;
				if (i == SERVER_INDEX)//if server is ready --> new connection
				{
					//add client
					std::cout << "new connection to server" << std::endl;
					server.addClient();
				}
				else //client is ready --> new connection
				{
					server._recv(i, buffer);
					//recv en boucle tant ret > 0 
					std::cout << "Message received from client: " << std::endl << buffer << std::endl;
				}
			}
			//if (server.checkSocket(i, POLLHUP)) //add POLLHUP to poll
		}
	}


}
