#include "Network.hpp"

Network::Network(void)	: _nb(0)
{
	bzero(_pfds, sizeof(struct pollfd) * SOMAXCONN);
}


void	Network::addSocket(int socket)
{
	_pfds[_nb].fd = socket;
	_pfds[_nb].events = POLLIN;
	_nb++;
} 
