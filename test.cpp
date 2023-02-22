/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qcherel <qcherel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 15:51:27 by qcherel           #+#    #+#             */
/*   Updated: 2023/02/22 17:08:01 by qcherel          ###   ########.fr       */
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


int socket_init(unsigned short port)
{
	//Create new socket (fd) with TCP (AF_INET) and 2-ways coms (SOCK_STREAM)
	std::cout << "A" << std::endl;

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	//Create struct sockaddr to bind the socket and his params (ports, AF_INET, ips)?
	struct sockaddr_in sin;
	//Protection if socketing failed;
	if (sock == -1)
	{
		perror("in socket():");
		exit(0);
	}
	std::cout << "A" << std::endl;
	//Convert INADDR_ANY(server accepts all addresses) from host readable (bytes-wise) to network readable (bytes-wise)
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	//Same but convert Port which is a short.
	sin.sin_port = htons(port);
	//same category as socket to bind.
	sin.sin_family = AF_INET;
	std::cout << "B" << std::endl;

	//Binding socket to sockaddr (pas compris pourquoi mais obligatoire) plus protection if fail
	if(bind(sock,(struct sockaddr *) &sin, sizeof sin) == -1)
	{
		perror("in bind():");
		exit(0);
	}
	std::cout << "C" << std::endl;
	//Mark socket as passive and to be used to accept() incoming connection requests
	if(listen(sock, 5/*max_requests*/) == -1)
	{
		perror("in bind():");
		exit(0);
	}
	std::cout << "D" << std::endl;
	return (sock);
}

int main(int ac, char **av)
{
	if (ac < 2)
		return (1);
	int sock = socket_init(atoi(av[1]));
	printf("%d", sock);
	close(sock);
	return (0);
}