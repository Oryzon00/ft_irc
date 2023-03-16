# include "Server.hpp"


Server::Server(int port, std::string password)
	: _name("13-20h_IRC"), _socket(initServerSocket(port)), _password(password)
{
	_network.addSocket(_socket);
	_clients.push_back(Client());
	initDico();
}


/* --------------------------------------------------------------------------------- */

/* ----- PRIVATE FUNCTION ----- */

std::string						Server::findKey(std::string cmd)
{
	char* key = strtok(const_cast<char *>(cmd.c_str()), " ");
	if (key && key[0] == ':')
		key = strtok(NULL, " ");
	if (!key)
		return (std::string());
	return (key);
}

std::vector<std::string>		Server::findArgsCmd(std::string cmd, std::string key)
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

void							Server::initDico(void)
{
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("CAP"), &Server::cmd_CAP));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("PASS"), &Server::cmd_PASS));

}

void							Server::callFunCmd(cmdFunction f, Client & client)
{
	(this->*(f))(client.getCmd(), client);
}


const std::string		Server::prefixServer(void) const
{
	return (":" + _name);
}

/* --------------------------------------------------------------------------------- */



/* GETTER */
	
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

/* PUBLIC FUNCTION */

void						Server::poll(void)
{
	_network._poll();
}


int							Server::readQuery(size_t index, char* buffer)
{
	Client&	client = _clients[index];

	bzero(buffer, BUFFER_LEN);
	int ret = recv(_network[index].fd, buffer, BUFFER_LEN, 0);
	if (errno != SUCCESS && errno != EAGAIN)
		throw SocketException("recv()");
	if (ret == DISCONNECT)
		return (DISCONNECT);

	client.readBuffer(buffer);
	client.findCmdInPackage();
	
	client.printCmd();
	client.printPackage();

	return (ret);
}

void						Server::processQuery(int index)
{
	Client&											client = _clients[index];
	std::string 									key = findKey(client.getCmd());
	std::map<std::string, cmdFunction>::iterator	it = _dico.find(key);

	if (!checkCAP(client, key))
		return ;
	if (it == _dico.end()) //repondre avec un code erreur??
	{
		//appeler ERR_UNKNOWNCOMMAND
		std::cerr << "Cmd:" << key << " not supported" << std::endl;
		client.clearCmd();
	}
	else
	{
		try
		{
			callFunCmd(it->second, client);
			client.findCmdInPackage();
			if (checkAnswerQuery(index))
				processQuery(index);
		}
		catch(const ClientDestroyedException& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
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





bool						Server::checkAnswerQuery(size_t index)
{
	Client&	client = _clients[index];
	return (checkSocket(index, POLLOUT) && client.checkCmdReady());
}


/* --------------------------------------------------------------------------------- */
