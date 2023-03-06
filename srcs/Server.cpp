# include "Server.hpp"

void	Server::addClient(Client client)
{
	_clients.push_back(client);
}
		
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
