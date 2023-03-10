# include "Server.hpp"


Server::Server(int port, std::string password)	: _socket(initServerSocket(port)), _password(password)
{
	_network.addSocket(_socket);
	_clients.push_back(Client());
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

int							Server::readPackages(size_t index, char* buffer)
{
	int ret = 1;

	do
	{
		bzero(buffer, BUFFER_LEN);
		ret = recv(_network[index].fd, buffer, BUFFER_LEN, 0);
		if (ret <= 0)
			break;
		_clients[index].readFromClient(buffer);
	}
	while (ret > 0);
	
	if (ret != 0)
		_clients[index].tokenizePack();

	std::cout  << "-----RECEIVED-----" << std::endl;
	for(size_t i = 0; i < _clients[index].getCmds().size(); i++)
		std::cout << i << ": " << _clients[index].getCmds()[i] << std::endl;
	std::cout << "-----END-----" << std::endl << std::endl;

	return (ret);
}

void						Server::sendPackages(size_t index)
{
	
	_clients[index].sendToClient();
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

void						Server::removeClient(size_t index)
{
	_clients.erase(_clients.begin() + index);
	_network.removeSocket(index);
}

void					Server::processQuery(int index)
{
	_clients[index].setToSend(_clients[index].getPackages());

	sendPackages(index);

}
