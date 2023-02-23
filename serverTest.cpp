//
// Created by lheurtev on 2/21/23.
//

#include <cstdlib>
#include <iostream>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


int initSocket (unsigned short port)
{
	int opt = 1;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror("socket()");
		return (1);
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		return (1);
	}

	struct sockaddr_in sin;
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if (bind (sock, (struct sockaddr  *) &sin, sizeof (sin)) == -1) {
		perror("bind()");
		return (1);
	}

	if (listen(sock, 5) == -1) {
		perror("listen()");

		return (1);
	}
	int csock;
	struct sockaddr_in csin = {0};
	socklen_t sinsize = sizeof (csin);
	csock = accept(sock, (struct sockaddr *)&csin, &sinsize);
	if (csock == -1)
	{
		perror("accept()");
		return (1);
	}
	char buff[1024] = {0};
	recv(csock, buff, 1023, 0);
	std::string str = buff;
	std::cout << str << std::endl;

	close(csock);
	close(sock);
	return (0);
}

int main (int ac, char **av)
{
	if (ac == 1)
		return (1);
	int port = atoi(av[1]);
	int clientSocket = initSocket(port);
	if (clientSocket)
		std::cout << "Error while connectiong socket" << std::endl;
	else
		std::cout << "Client connected to port : " << port << std::endl;
	return (0);
}