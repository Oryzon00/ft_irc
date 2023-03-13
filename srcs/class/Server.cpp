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

void						Server::removeClient(Client &client)
{
	std::vector<Client>::iterator	it = std::find(_clients.begin(), _clients.end(), client);
	long index = it - _clients.begin();
	_clients.erase(it);
	_network.removeSocket(index);
}


void						Server::processQuery(int index)
{
	Client&	client = _clients[index]; //est ce qu'n a une copie ? ou le vrai client
	

	for (itVector it = client.getCmds().begin(); it != client.getCmds().end(); it++)
	{
		std::string key = getKey(*it);
		std::map<std::string, cmdFunction>::iterator itFind = _dico.find(key);
		if (itFind == _dico.end())
			std::cerr << "Cmd:" << key << " not found" << std::endl;
		else if (!checkCAP(client, key))
			return ;
		else
			*(itFind->second)(*it, client);
	}
	client.setToSend(client.getPackages()); //to delete
	sendPackages(client);
}

bool						Server::checkCAP(Client &client, std::string key)
{
	if (client.getIsIrssi() == false && key != "CAP")
	{
		removeClient(client);
		std::cerr << "!! -- First CMD is not CAP -- !!" << std::endl;
		return (false);
	}
	return (true);
}


/* --------------------------------------------------------------------------------- */

/* ----- PRIVATE FUNCTION ----- */

void							Server::initDico(void)
{
	// std::pair<std::string, cmdFunction> myPair(std::string("CAP"), &Server::cmd_CAP);
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("CAP"), &Server::cmd_CAP));
}

std::string						Server::getKey(std::string cmd)
{
	char* key = strtok(const_cast<char *>(cmd.c_str()), " ");
	if (key && key[0] == ':')
		key = strtok(NULL, " ");
	if (!key)
		return (std::string());
	return (key);
}

std::vector<std::string>		Server::getArgsCmd(std::string cmd, std::string key)
{
	std::vector<std::string>	args;

	char	*token = strtok(const_cast<char*>(cmd.c_str() + cmd.find(key) + key.length()), " ");
	if (token)
		args.push_back(std::string(token));
	while (token)
	{
		token = strtok(NULL, " "); 
		if (token)
			args.push_back(std::string(token));
	}
	return (args);
}

void	Server::cmd_CAP(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = getArgsCmd(cmd, "CAP");
	if (args.size() != 1 || args[0] != "LS")
	{
		removeClient(client);
		std::cerr << "!! -- Client CAP is not LS -- !!" << std::endl;
	}

	client.setIsIrssi(true);
}
