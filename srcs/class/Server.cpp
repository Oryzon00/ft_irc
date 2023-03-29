# include "Server.hpp"

Server::Server(int port, std::string password)
	:  _name("13.20h"), _socket(initServerSocket(port)), _id_count(1), _password(password), _MOTD(ASCII_COMPUTER)
{
	_network.addSocket(_socket);
	_clients.push_back(Client());
	initDico();
	initBot();
}

void	Server::initBot()
{
	_bot.setSocket(-1);
	_bot.setNickname("Mr.Bot");
	_bot.setUsername("Mr.Bot");
	_bot.setRealname("Mister Bot");

	Channel	chan_bot(&_bot, "#bot", "");
	chan_bot.setTopic("Come interact with Mister Bot (!help) :)");
	_chans.push_back(chan_bot);
}

Server::~Server(void)
{

}

/* --------------------------------------------------------------------------------- */

/* ----- PRIVATE FUNCTION ----- */


Client*							Server::find_client_by_nick(std::string nick)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getNickname() == nick)
			return (&(*it));
	}
	return (NULL);
}

Client*							Server::find_client_by_id(unsigned long id)
{
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if (it->getID() == id)
			return (&(*it));
	}
	return (NULL);
}

Channel*						Server::findChannel(std::string name)
{
	for (std::vector<Channel>::iterator it = _chans.begin(); it != _chans.end(); it++)
	{
		if (it->getName() == name)
			return (&(*it));
	}
	return (NULL);
}

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
	std::string					str;

	char	*token = strtok(const_cast<char*>(cmd.c_str() + cmd.find(key) + key.length()), " ");
	if (token)
		args.push_back(std::string(token));
	
	while (token)
	{
		token = strtok(NULL, " ");
		if (token)
		{
			str = token;
			if (str.find(':') == 0)
			{
				do
				{
					token = strtok(NULL, " ");
					if (token)
						str += std::string(" ") + std::string(token); 
				} while (token);
			}
			args.push_back(str);
		}	
	}
	return (args);
}

bool						Server::checkAvailNick(const std::string str)
{
	for(std::vector<Client>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
	{
		if (str == it->getNickname())
			return (false);
	}
	return (true);
}

bool						Server::checkValidNick(const std::string& str)
{
	if (str.empty() || str == "*" || str.length() >= 20 || str[0] == '#')
		return (false);
	return (true);
}

bool						Server::validChannelName(const std::string& name)
{
	if (name.empty() || name == "*" || name.length() >= 20 || name[0] != '#')
		return (false);
	return (true);
}

bool						Server::checkOP(Client& client, Channel& channel)
{
	return(client.getModeO() || channel[0] == client.getID());
}


bool						Server::checkCAP(Client &client, std::string key)
{
	if (client.getIsIrssi() == false && key != "CAP")
	{
		removeClient(client);
		std::cout << "!! -- First CMD is not CAP -- !!" << std::endl;
		return (false);
	}
	return (true);
}

bool							Server::checkRegistered(cmdFunction f, Client & client)
{
	if (f != &Server::cmd_CAP && f != &Server::cmd_PASS && f != &Server::cmd_NICK && f != &Server::cmd_USER
		&& !client.getModeR())
			return (false);
	return (true);
}

void							Server::callFunCmd(cmdFunction f, Client & client)
{
	if (!checkRegistered(f, client))
	{
		f_ERR_NOTREGISTERED(client);
		quitClientCmd(client);
	}

	(this->*(f))(client.getCmd(), client);
	client.clearCmd();
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
	// if (errno != SUCCESS && errno != EAGAIN) //enlever sujet???
	// 	throw SocketException("recv()");
	if (ret == DISCONNECT)
		return (DISCONNECT);

	client.readBuffer(buffer);
	client.findCmdInPackage();
	client.printCmd(); // a enlever
	return (ret);
}

void						Server::processQuery(int index)
{
	Client&											client = _clients[index];
	std::string 									key = findKey(client.getCmd());
	std::map<std::string, cmdFunction>::iterator	it = _dico.find(key);

	try
	{
		if (it == _dico.end())
			f_ERR_UNKNOWNCOMMAND(client);
		else
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


bool						Server::checkSocket(size_t index, short event)
{
	return(_network[index].revents & event);
}

void						Server::addClient(void)
{
	_clients.push_back(Client(_socket, _id_count++));
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

void						Server::removeChannel(Channel channel)
{
	_chans.erase(std::find(_chans.begin(), _chans.end(), channel));
}




bool						Server::checkAnswerQuery(size_t index)
{
	Client&	client = _clients[index];
	return (checkSocket(index, POLLOUT) && client.checkCmdReady());
}

void						Server::sendToChannel(Channel* channel, Client& sender, const std::string& str)
{

	for(std::vector<unsigned long>::iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
	{
		Client* client = find_client_by_id(*it);
		if (client && *client != sender)
			client->sendToClient(str);
	}
}
/* --------------------------------------------------------------------------------- */
