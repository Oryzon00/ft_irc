#include "Network.hpp"

Network::Network(void)	: _nb(0)
{
	bzero(_pfds, sizeof(struct pollfd) * SOMAXCONN);
}

Network::~Network(void)
{
	for(size_t i = 0; i < _nb; i++)
		close(_pfds[i].fd);
}


void	Network::addSocket(int socket)
{
	_pfds[_nb].fd = socket;
	_pfds[_nb].events = POLLIN; 
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

void			Network::removeSocket(size_t index)
{
	close(_pfds[index].fd);
	for(size_t i = index + 1; i < _nb; i++)
		_pfds[i - 1] = _pfds[i];
	_pfds[_nb].fd = 0;
	_pfds[_nb].events = 0;
	_pfds[_nb].revents = 0;
	_nb--;
}
