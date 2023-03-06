#include "irc.hpp"
#include <sys/socket.h>
#include <netinet/in.h>


int	initServerSocket(unsigned short port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw SocketException("socket()");
	
	int	enable = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int)))
		throw SocketException("setsockopt()");
	
	struct sockaddr_in sin;
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)))
		throw SocketException("bind()");
	
	if (listen(sock, SOMAXCONN))
		throw SocketException("listen()");
	
	return (sock);
}
