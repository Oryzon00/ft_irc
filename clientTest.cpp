//
// Created by lheurtev on 2/24/23.
//

#include <cstdlib>
#include <iostream>

#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int initConnection (unsigned short port) {
	int opt = 1;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("socket()");
		return (-1);
	}

	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		return (-1);
	}

	struct sockaddr_in sin;
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("connect()");
		return (-1);
	}
	return (sock);
}

int sendMessage (int servSocket)
{
	std::string buff;
	std::cout << "COMMAND: ";
	std::getline (std::cin, buff);
	if (send(servSocket, buff.c_str(), buff.size(), 0) < 0)
		return (-1);
	return (0);
}

int main (int ac, char **av)
{
	if (ac == 1)
		return (1);
	int port = atoi(av[1]);
	int serverSocket = initConnection(port);
	if (serverSocket < 0) {
		std::cout << "ERROR SERVER SOCKET CREATION" << std::endl;
		return (1);
	}
	while (1) {
		if (sendMessage(serverSocket) < 0)
			break;
	}
	close(serverSocket);
	return (0);
}