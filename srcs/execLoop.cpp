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
				// std::cout << "------ fd nb " << index << " ready to read ------" << std::endl << std::endl;
				if (index == SERVER_INDEX)
				{
					std::cout << "---- new connection to server ----" << std::endl;
					server.addClient();
				}
				else 
				{
					std::cout << "---- client connection number: ----" << index << std::endl;
					if (server.readQuery(index, buffer) == DISCONNECT)
					{
						std::cout << "---- client disconnected ----" << std::endl;
						server.removeClient(index);
					}
					else if (server.checkAnswerQuery(index))
					{
						std::cout << "---- processQuery of client ----" << std::endl;
						server.processQuery(index);
					}
					
				}
			}
			
		}
		
	}
}
