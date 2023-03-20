#include "irc.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>


int	initClientSocket(int socket_server)
{
	struct sockaddr_in client_sin;
	socklen_t size = sizeof(client_sin);
	
	int client_socket = accept(socket_server, (sockaddr *)  &client_sin, &size);
	if (client_socket < 0)
		throw SocketException("accept()");

	if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
	{
		close(client_socket);
		throw SocketException("fcntl()");
	}
		
	return (client_socket);
}

int	initServerSocket(unsigned short port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0)
		throw SocketException("socket()");

	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
	{
		close(sock);
		throw SocketException("fcntl()");
	}

	int	enable = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &enable, sizeof(int)))
	{
		close(sock);
		throw SocketException("setsockopt()");
	}
	
	struct sockaddr_in sin;
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)))
	{
		close(sock);
		throw SocketException("bind()");
	}

	if (listen(sock, SOMAXCONN))
	{
		close(sock);
		throw SocketException("listen()");
	}
	return (sock);
}
