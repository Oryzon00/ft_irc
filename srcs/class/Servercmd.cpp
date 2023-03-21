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
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("JOIN"), &Server::cmd_JOIN));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("QUIT"), &Server::cmd_QUIT));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("OPER"), &Server::cmd_OPER));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("MODE"), &Server::cmd_MODE));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("kill"), &Server::cmd_KILL));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("restart"), &Server::cmd_RESTART));
	
}

void	Server::welcomeClient(Client &client)
{
	reply_handler(RPL_WELCOME, client);
	reply_handler(RPL_YOURHOST, client);
	reply_handler(RPL_CREATED, client);
	reply_handler(RPL_MYINFO, client);
	reply_handler(RPL_ISUPPORT, client); // a finir en fin de projet

	//RPL_UMODEIS or cmd_MODE on user

	error_handler(ERR_NOMOTD, client);
}

/* --------------------------------------------------------------------------------- */

/* CMD */

void	Server::cmd_MODE_user(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void)  cmd;
	std::string	nick = args[0];
	Client*		target = find_client_by_nick(nick);

	if (!target)
		error_handler(ERR_NOSUCHNICK, client);
	else if (nick != client.getNickname())
		error_handler(ERR_USERSDONTMATCH, client);
	else
		return ;

}

void	Server::cmd_MODE_channel(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void) cmd;
	(void) client;
	(void)args;
}


void	Server::cmd_MODE(std::string& cmd, Client & client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "MODE");
	if (args.size() == 0)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (args[0][0] != '#')
		cmd_MODE_user(cmd, client, args);
	else if (args[0][0] == '#')
		cmd_MODE_channel(cmd, client, args);
	
}

void	Server::cmd_RESTART(std::string& cmd, Client& client)
{
	(void) cmd;
	if (!client.getModeO())
		error_handler(ERR_NOPRIVILEGES, client);
	else
		throw RestartException();
}

void	Server::cmd_CAP(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "CAP");
	if (args.size() != 1 || args[0] != "LS")
	{
		removeClient(client);
		std::cout << "!! -- Client CAP is not LS -- !!" << std::endl;
	}
	client.setIsIrssi(true);
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
}

void	Server::cmd_PING(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PING");

	if (args.size() != 1)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
		client.sendToClient(prefixServer() + " PONG " + _name + " :" + args[0] + "\n");
}

void							Server::cmd_QUIT(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "QUIT");

	std::string	str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " QUIT ";
	if (!args.empty())
		str += args[0] + "\n";
	else
		str += ":left without a reason :o\n";

	for (std::vector<Client>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
		it->sendToClient(str);
	quitClientCmd(client);
}

void	Server::cmd_USER(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "USER");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (client.getModeR())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args.size() != 4)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		client.setUsername(args[0]);
		client.setRealname(args[3].substr(1, std::string::npos));
		client.setModeR(true);
		welcomeClient(client);
	}
	client.clearCmd();
}

void	Server::join_channel(Client& client, std::string name, std::string key)
{
	Channel* channel = findChannel(name);
	if (!channel && validChannelName(name))
	{
		_chans.push_back(Channel(client, name, key));
		client.sendToClient(":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " JOIN :" + name + "\n");
		reply_handler(RPL_TOPIC, client, name);
		reply_handler(RPL_NAMREPLY, client, name);
		reply_handler(RPL_ENDOFNAMES, client, name);
	}
	else if (!channel)
		error_handler(ERR_BADCHANMASK, client, name);
	else if (key != channel->getKey())
		error_handler(ERR_BADCHANNELKEY, client, name);
	else
	{
		channel->addMember(client);
		channel->SendToAll(":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " JOIN :" + name + "\n");
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
			join_channel(client, chans[i], (i < keys.size()) ? keys[i] : "x");
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
		client_oper->setModeO(true);
	}
}

void	Server::cmd_KILL(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "kill");
	Client*						client_cible = NULL;
	std::string					cible_nick;
	std::string					comment;

	if (args.size() == 2)
	{
		cible_nick = args[0];
		comment = args[1];
		client_cible = find_client_by_nick(cible_nick);
	}

	if (args.size() != 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (!client.getModeO())
		error_handler(ERR_NOPRIVILEGES, client);
	else if (!client_cible)
		error_handler(ERR_NOSUCHNICK, client);
	else
	{
		for (std::vector<Client>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
			f_RPL_KILLREPLY(*it, cible_nick, client, comment);
		if (client_cible)
		{
			client.clearCmd();
			quitClientCmd(*client_cible);
		}
	}
}
