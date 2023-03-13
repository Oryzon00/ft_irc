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

int							Server::readQuery(size_t index, char* buffer)
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

void						Server::sendPackages(Client & client)
{
	
	client.sendToClient(); // a tester
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

std::string					Server::getKey(std::string& cmd)
{
	return (cmd);
	// a coder
}

void					Server::processQuery(int index)
{
	Client&	client = _clients[index]; //est ce qu'n a une copie ? ou le vrai client
	client.setToSend(client.getPackages()); //to delete

	for (itVector it = client.getCmds().begin(); it != client.getCmds().end(); it++)
	{
		std::string key = getKey(*it);
		cmdFunction	cmdF = _dico[key]; //verifier retour erreur _dico[key]
		cmdF(*it, client); 
	}

	sendPackages(client);


}
