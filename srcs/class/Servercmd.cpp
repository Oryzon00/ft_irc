#include "Server.hpp"

void	Server::quitClientCmd(Client &client)
{
	removeClient(client);
	throw ClientDestroyedException();
}

void	Server::initDico(void)
{
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("CAP"), &Server::cmd_CAP));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("PASS"), &Server::cmd_PASS));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("NICK"), &Server::cmd_NICK));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("USER"), &Server::cmd_USER));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("PING"), &Server::cmd_PING));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("OPER"), &Server::cmd_OPER));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("JOIN"), &Server::cmd_JOIN));

}

/* --------------------------------------------------------------------------------- */

/* CMD */

void	Server::cmd_CAP(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "CAP");
	if (args.size() != 1 || args[0] != "LS")
	{
		removeClient(client);
		std::cerr << "!! -- Client CAP is not LS -- !!" << std::endl;
	}
	client.setIsIrssi(true);
	client.clearCmd();
}

void	Server::cmd_PASS(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PASS");
	if (args.empty())
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (client.getPassOk())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args[0] != _password)
	{
		error_handler(ERR_PASSWDMISMATCH, client);
		quitClientCmd(client);
	}
	else
		client.setPassOk(true);
	client.clearCmd();

}

void	Server::cmd_NICK(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "NICK");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (args.size() != 1)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (checkAvailNick(args[0]) == false)
		error_handler(ERR_NICKNAMEINUSE, client);
	else if (checkValidNick(args[0]) == false)
		error_handler(ERR_ERRONEUSNICKNAME, client);
	else
		client.setNickname(args[0]);
	std::cout << client.getNickname() <<std::endl;
	client.clearCmd();
}

void	Server::cmd_PING(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PING");

	if (args.size() != 1)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
		client.sendToClient(prefixServer() + " PONG " + _name + " :" + args[0] + "\n");
	client.clearCmd();
}

/*void							Server::cmd_QUIT(std::string& cmd, Client& client)
{
	//If client connected to channel, write in channel
}*/

void	Server::cmd_USER(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "USER");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (client.getRegistered())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args.size() != 4)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		client.setUsername(args[0]);
		client.setRealname(args[3].substr(1, std::string::npos));
		client.setRegistered(true);
	}
	client.clearCmd();
	//client.check registration
}

void	Server::join_channel(Client& client, std::string name, std::string key)
{
	Channel* channel = findChannel(name);
	if (!channel && validChannelName(name))
	{
		_chans.push_back(Channel(client, name, key));
		client.sendToClient(client.getNickname() + " JOIN " + name + "\n");
		reply_handler(RPL_TOPIC, client, name);
		reply_handler(RPL_NAMREPLY, client, name);
		reply_handler(RPL_ENDOFNAMES, client, name);
	}
	else if (!channel)
		error_handler(ERR_NOSUCHCHANNEL, client, name);
	else if (key != channel->getKey())
		error_handler(ERR_BADCHANNELKEY, client, name);
	else
	{
		channel->addMember(client);
		reply_handler(RPL_TOPIC, client, name);
		reply_handler(RPL_NAMREPLY, client, name);
		reply_handler(RPL_ENDOFNAMES, client, name);
	}
	//bans in channels?
}

void	Server::cmd_JOIN(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "JOIN");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (args.size() <= 2 && !args.empty())
	{
		std::vector<std::string>		chans = strToVec(args[0], ",");
		std::vector<std::string>		keys;
		if (args.begin() + 1 != args.end())
			keys = strToVec(args[1], ",");
		for(size_t i = 0; i < chans.size(); i++)
			join_channel(client, chans[i], (i < keys.size()) ? keys[i] : "");
	}
	else
		error_handler(ERR_WRONGNBPARAMS, client);
	client.clearCmd();
}

void	Server::cmd_OPER(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "OPER");
	Client*	client_oper = NULL;

	if (args.size() == 2)
		client_oper = find_client_by_nick(args[0]);
	if (args.size() != 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (args[1] != OPER_PASSWD)
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (client_oper == NULL)
		error_handler(ERR_NOOPERHOST, client);
	else
	{
		reply_handler(RPL_YOUREOPER, *client_oper);
		client.setOper(true);
	}
	client.clearCmd();
}


/* --------------------------------------------------------------------------------- */

/* ERR */

void	Server::error_handler(int ERR_CODE, Client &client, const std::string& str)
{
	switch (ERR_CODE)
	{
		case ERR_WRONGNBPARAMS:
			f_ERR_WRONGNBPARAMS(client);
			break;
		case ERR_ALREADYREGISTERED:
			f_ERR_ALREADYREGISTERED(client);
			break;
		case ERR_PASSWDMISMATCH:
			f_ERR_PASSWDMISMATCH(client);
			break;
		case ERR_NICKNAMEINUSE:
			f_ERR_NICKNAMEINUSE(client);
			break;
		case ERR_ERRONEUSNICKNAME:
			f_ERR_ERRONEUSNICKNAME(client);
			break;
		case ERR_NOOPERHOST:
			f_ERR_NOOPERHOST(client);
			break;
		case ERR_BADCHANNELKEY:
			f_ERR_BADCHANNELKEY(client, str);
			break;
		case ERR_NOSUCHCHANNEL:
			f_ERR_NOSUCHCHANNEL(client, str);
			break;
		default:
			break;
	}
}

void	Server::f_ERR_NOOPERHOST(Client &client) //a gerer?
{
	std::string code = " 491 ";
	std::string	str = prefixServer() + code + client.getNickname() + " "
		+ " :No O-lines for your host\n";
    client.sendToClient(str);
}

void	Server::f_ERR_WRONGNBPARAMS(Client &client)
{
	std::string code = " 461 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Wrong number of parameters\n";
    client.sendToClient(str);
}

void	Server::f_ERR_ALREADYREGISTERED(Client &client)
{
	std::string code = " 462 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :You may not reregister\n";
    client.sendToClient(str);
}

void	Server::f_ERR_PASSWDMISMATCH(Client &client)
{
	std::string code = " 464 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :Password incorrect\n";
    client.sendToClient(str);
}

void	Server::f_ERR_UNKNOWNCOMMAND(Client &client)
{
	std::string code = " 421 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Unknown command\n";
    client.sendToClient(str);
	client.clearCmd();
}

void							Server::f_ERR_NICKNAMEINUSE(Client &client)
{
	std::string code = " 433 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findArgsCmd(client.getCmd(), "NICK")[0] 
		+ " :Nickname is already in use\n";
    client.sendToClient(str);
}

void							Server::f_ERR_ERRONEUSNICKNAME(Client &client)
{
	std::string code = " 432 ";
	std::vector<std::string> args = findArgsCmd(client.getCmd(), "NICK");
	std::string str;
	if (args.size() == 1)
		str = prefixServer() + code + client.getNickname() + " " + args[0] 
			+ " :Erroneus nickname\n";
    else
		str = prefixServer() + code + client.getNickname() + args[0] + " " + args[1]
			+ " :Erroneus nickname\n";
	client.sendToClient(str);
}

void							Server::f_ERR_NOSUCHCHANNEL(Client &client, const std::string& channel_name)
{
	std::string code = " 403 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Channel Invalid\n";
    client.sendToClient(str);
}


void							Server::f_ERR_BADCHANNELKEY(Client &client, const std::string& channel_name)
{
	std::string code = " 475 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Cannot join channel (Wrong Key)\n";
    client.sendToClient(str);
}



/* --------------------------------------------------------------------------------- */


/* RPL */


void	Server::reply_handler(int RPL_CODE, Client &client, const std::string& str)
{
	switch (RPL_CODE)
	{
		case RPL_YOUREOPER:
			f_RPL_YOUREOPER(client);
			break;
		case RPL_TOPIC:
			f_RPL_TOPIC(client, str);
			break;
		case RPL_NAMREPLY:
			f_RPL_NAMREPLY(client, str);
			break;
		case RPL_ENDOFNAMES:
			f_RPL_ENDOFNAMES(client, str);
			break;
		default:
			break;
	}
}

void	Server::f_RPL_YOUREOPER(Client &client)
{
	std::string					code = " 381 ";

	std::string	str = prefixServer() + code + client.getNickname() + " "
		+ ":You are now an IRC operator\n";
	client.sendToClient(str);
}

void	Server::f_RPL_TOPIC(Client &client, const std::string& channel_name)
{
	std::string					code = " 332 ";

	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name
		+ " :" + findChannel(channel_name)->getTopic() + "\n";
	client.sendToClient(str);
}

void	Server::f_RPL_NAMREPLY(Client &client, const std::string& channel_name)
{
	std::string					code = " 353 ";

	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name
		+ ":List of Names in channel\n"; // A finir
	client.sendToClient(str);
}

void	Server::f_RPL_ENDOFNAMES(Client &client, const std::string& channel_name)
{
	std::string					code = " 366 ";

	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name
		+ ":END of /NAMES list\n";
	client.sendToClient(str);
}
