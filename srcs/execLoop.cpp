#include "irc.hpp"
#include <poll.h>
# include <sys/socket.h>
#include <unistd.h>
#include <strings.h>

int	initClientSocket(int socket_server);

void	execLoop(Server &server)
{
	char	buffer[BUFFER_LEN] = {0};

	while (42)
	{
		server.poll();
		for(size_t i = 0; i < server.getNetwork().size(); i++)
		{
			if (server.checkSocket(i, POLLIN))
			{
				std::cout << "------ fd nb " << i << " ready ------" << std::endl;
				if (i == SERVER_INDEX)
				{
					std::cout << "new connection to server" << std::endl;
					server.addClient();
				}
				else 
				{
					if (server.readPackages(i, buffer) == DISCONNECT)
					{
						std::cout << "client disconnected" << std::endl;
						server.removeClient(i);
					}
						
					
				}
			}
			// if (server.checkSocket(i, POLLHUP)) //add POLLHUP to poll
			// {
			// 	std::cout << "hola" << std::endl;
			// }
		}
		//ON AGIT
		sleep (1);
	}


}
