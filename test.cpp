/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qcherel <qcherel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:51:27 by qcherel           #+#    #+#             */
/*   Updated: 2023/03/08 12:24:22 by qcherel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <iostream>
#include <string>
#include <vector>


int socket_init(unsigned short port)
{
	int opt = 1;
	//Create new socket (fd) with TCP (AF_INET) and 2-ways coms (SOCK_STREAM)
	// std::cout << "A" << std::endl;
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	//Create struct sockaddr to bind the socket and his params (ports, AF_INET, ips)?
	struct sockaddr_in sin;
	//Protection if socketing failed;
	if (sock == -1)
	{
		perror("in socket():");
		exit(0);
	}
	//Change socket options 
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("in setsockopt()");
		return (1);
	}
	//Convert INADDR_ANY(server accepts all addresses) from host readable (bytes-wise) to network readable (bytes-wise)
	sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	//Same but convert Port which is a short.
	sin.sin_port = htons(port);
	//same category as socket to bind.
	sin.sin_family = AF_INET;

	//Binding socket to sockaddr (pas compris pourquoi mais obgatoire) plus protection if fail
	if(bind(sock,(struct sockaddr *) &sin, sizeof sin) == -1)
	{
		perror("in bind()");
		exit(0);
	}
	//Mark socket as passive and to be used to accept() incoming connection requests
	if(listen(sock, 5/*max_requests*/) == -1)
	{
		perror("in bind():");
		exit(0);
	}
	return (sock);
}

int create_new_client(int sock, std::string &name)
{
	int client_socket;
	struct sockaddr_in client_sin;
	socklen_t size = sizeof(client_sin);
	
	client_socket = accept(sock, (sockaddr *)  &client_sin, &size);
	if (client_socket == -1)
	{
		perror("in accept()");
		exit(0);
	}
	char buff[1024] = {0};
	recv(client_socket, buff, 1023, 0);
	name = buff;
	name.erase(name.begin(), name.begin() + name.find("NICK") + 5);
	name.erase(name.begin() + name.find("USER"), name.end());
	return (client_socket);
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (1);
	//Make the socket which will receive external connections
	int sock = socket_init(atoi(av[1]));
	int last_fd = sock;
	//Array of client sockets
	std::vector<int> client_sock(5);
	int connected_clients = 0;
	std::vector<std::string>  username(5);
	
	fd_set fds;
	//Server Loop
	while (1)
	{
		int i = -1;
		//Reset socket set;
		FD_ZERO(&fds);
		//Add stdin (used later to close server properly) to the socket set
		FD_SET(STDIN_FILENO, &fds);
		//Add the server socket to the socket set
		FD_SET(sock, &fds);
		//Add all connected clients to the socket set
		while (++i < connected_clients)
			FD_SET(client_sock[i], &fds);
		//read on all FD from the socket set and keep only those with updates
		if(select(last_fd + 1, &fds, NULL, NULL, NULL) == -1)
		{
			perror("in select()");
			exit(errno);
		}
		
		if (FD_ISSET(STDIN_FILENO, &fds)) //Receiving something on STDIN (user inputs on server terminal)
			exit(1);
		else if (FD_ISSET(sock, &fds)) // Receiving something on server socket (a new client is trying to connect)
		{
			//Add a socket for the new client
			client_sock[connected_clients] = create_new_client(sock, username[connected_clients]);
			std::cout << username[connected_clients] << std::endl;
			if (last_fd < client_sock[connected_clients])
				last_fd = client_sock[connected_clients];
			//Add the newly created socket to the socket set
			FD_SET(client_sock[connected_clients], &fds);
			//A new client is connected and registered
			connected_clients++;
		}
		else
		{
			i = -1;
			while (++i < connected_clients)
			{
				if (FD_ISSET(client_sock[i], &fds))
				{
					if ()
				}
			}
			//Read and output connected client sockets	
		}
		//A implementer : client read and send, client deconnection, close sockets if stdin
	}










	/*int client_socket;
	struct sockaddr_in client_sin;
	socklen_t size = sizeof(client_sin);
	
	client_socket = accept(sock, (sockaddr *)  &client_sin, &size);
	if (client_socket == -1)
	{
		perror("in accept()");
		exit(0);
	}*/


	close(sock);
	// close(client_socket);
	return (0);
}