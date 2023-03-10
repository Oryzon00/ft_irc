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
		for(size_t index = 0; index < server.getNetwork().size(); index++)
		{
			if (server.checkSocket(index, POLLIN))
			{
				std::cout << "------ fd nb " << index << " ready ------" << std::endl;
				if (index == SERVER_INDEX)
				{
					std::cout << "new connection to server" << std::endl;
					server.addClient();
				}
				else 
				{
					if (server.readPackages(index, buffer) == DISCONNECT)
					{
						std::cout << "client disconnected" << std::endl;
						server.removeClient(index);
					}
					else if (server.checkSocket(index, POLLOUT))
					{
						std::cout << "on peut repondre au socket" << std::endl;
						// int ret = send(server.getClients()[index].getSocket(), server.getClients()[index].getPackages().c_str(), server.getClients()[index].getPackages().size(), 0);
						// std::cout << "retour de send() " << ret << std::endl;
						server.sendPackages(index);
					}
					
				}
			}
		}
		sleep (1);
	}


}
