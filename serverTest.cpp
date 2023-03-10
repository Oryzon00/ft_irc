#include <cstdlib>
#include <iostream>

#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

struct s_client {
	int sock;
	char buff[1024] = {0};
} typedef t_client;

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

	if (bind(sock, (struct sockaddr *) &sin, sizeof(sin)) == -1) {
		perror("bind()");
		return (-1);
	}

	if (listen(sock, 5) == -1) {
		perror("listen()");
		return (-1);
	}
	return (sock);
}

int initSocket (int servSock) {

	int csock;
	struct sockaddr_in csin = {0};
	socklen_t sinsize = sizeof(csin);
	csock = accept(servSock, (struct sockaddr *) &csin, &sinsize);
	if (csock == -1) {
		perror("accept()");
		return (-1);
	}
	return (csock);
}

int mirror (int clientSock) {
	char buff[1024] = {0};
	if (recv(clientSock, buff, 1023, 0) == 0) {
		//perror("recv()");
		return (-1);
	}
	if (send(clientSock, buff, strlen(buff), 0) < 0) {
		perror("send()");
		return (-1);
	}
	return (0);
}

int main (int ac, char **av)
{
	//t_client clients[5];
	if (ac == 1)
		return (1);
	int port = atoi(av[1]);
	int serverSocket = initConnection(port);
	if (serverSocket < 0) {
		std::cout << "ERROR SERVER SOCKET CREATION" << std::endl;
		return (1);
	}
	int clientSocket = initSocket(serverSocket);
	if (clientSocket < 0)
	{
		std::cout << "ERROR CLIENT SOCKET CREATION" << std::endl;
		close(serverSocket);
		return (1);
	}
	while (1) {
		if (mirror(clientSocket) < 0)
			break;
	}
	close(clientSocket);
	close(serverSocket);
	return (0);
}