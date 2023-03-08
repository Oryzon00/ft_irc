# include "Server.hpp"


Server::Server(int port, std::string password)	: _socket(initServerSocket(port)), _password(password)
{
	_network.addSocket(_socket);
}



/* --------------------------------------------------------------------------------- */

	
const int&					Server::getServerSocket(void) const
{
	return _socket;
}
const std::string&			Server::getPassword(void) const
{
	return _password;
}

const std::vector<Client>&	Server::getClients(void) const
{
	return _clients;
}

const Network&				Server::getNetwork(void) const
{
	return _network;
}

/* --------------------------------------------------------------------------------- */

void						Server::poll(void)
{
	_network._poll();
}

size_t						Server::_recv(size_t index, char* buffer)
{
	size_t	ret = true;
	while (ret)
	{
		ret = recv(_network[index].fd, buffer, sizeof(buffer), 0);
		if (ret < 0)
			throw SocketException("recv()");
		
	}
	return (ret);
}

bool						Server::checkSocket(size_t index, short event)
{
	return(_network[index].revents & event);
}

void						Server::addClient(void)
{
	_clients.push_back(Client(_socket));
	_network.addSocket(_clients.back().getSocket());
}
