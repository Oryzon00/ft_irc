#include "Network.hpp"

Network::Network(void)	: _nb(0)
{
	bzero(_pfds, sizeof(struct pollfd) * SOMAXCONN);
}


void	Network::addSocket(int socket)
{
	_pfds[_nb].fd = socket;
	_pfds[_nb].events = POLLIN; //POLLHUP
	_nb++;
} 

void	Network::_poll(void)
{
	if (poll(_pfds, _nb, 100) < 0)
		throw SocketException("poll()");
}

size_t	Network::size(void) const
{
	return(_nb);
}

struct pollfd&	Network::operator[](size_t index)
{
	return(_pfds[index]);
}