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
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("kill"), &Server::cmd_KILL));
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

void	Server::cmd_CAP(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "CAP");
	if (args.size() != 1 || args[0] != "LS")
	{
		removeClient(client);
		std::cerr << "!! -- Client CAP is not LS -- !!" << std::endl;
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
	else if (client.getRegistered())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args.size() != 4)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		client.setUsername(args[0]);
		client.setRealname(args[3].substr(1, std::string::npos));
		client.setRegistered(true);
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
		client_oper->setOper(true);
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
		comment = args[1]; //j'espere que comment commence par un  :
		client_cible = find_client_by_nick(cible_nick);
	}

	if (args.size() != 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (!client.getOper())
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
		case ERR_NOMOTD:
			f_ERR_NOMOTD(client);
			break;
		case ERR_NOOPERHOST:
			f_ERR_NOOPERHOST(client);
			break;
		case ERR_BADCHANNELKEY:
			f_ERR_BADCHANNELKEY(client, str);
			break;
		case ERR_BADCHANMASK:
			f_ERR_BADCHANMASK(client, str);
			break;
		case ERR_NOPRIVILEGES:
			f_ERR_NOPRIVILEGES(client);
			break;
		case ERR_NOSUCHNICK:
			f_ERR_NOSUCHNICK(client);
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

void	Server::f_ERR_NICKNAMEINUSE(Client &client)
{
	std::string code = " 433 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findArgsCmd(client.getCmd(), "NICK")[0] 
		+ " :Nickname is already in use\n";
    client.sendToClient(str);
}

void	Server::f_ERR_ERRONEUSNICKNAME(Client &client)
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

void							Server::f_ERR_BADCHANMASK(Client &client, const std::string& channel_name)
{
	std::string code = " 476 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Bad Channel Mask\n";
    client.sendToClient(str);
}


void							Server::f_ERR_BADCHANNELKEY(Client &client, const std::string& channel_name)
{
	std::string code = " 475 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Cannot join channel (Wrong Key)\n";
    client.sendToClient(str);
}
	
void	Server::f_ERR_NOMOTD(Client &client)
{
	std::string code = " 422 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :MOTD is not supported\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOPRIVILEGES(Client &client)
{
	std::string code = " 481 ";
	std::string str = prefixServer() + code + client.getNickname() + " " 
		+ ":Permission Denied - You're not an IRC operator\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOSUCHNICK(Client & client)
{
	std::string code = " 401 ";
	std::vector<std::string> args = findArgsCmd(client.getCmd(), "kill");
	std::string str = prefixServer() + code + client.getNickname() + " " + args[0] + " "
		+ ":No such nick\n";
	client.sendToClient(str);
}



/* --------------------------------------------------------------------------------- */


/* RPL */


void	Server::reply_handler(int RPL_CODE, Client &client, const std::string& str)
{
	switch (RPL_CODE)
	{
		case RPL_WELCOME:
			f_RPL_WELCOME(client);
			break;
		case RPL_YOURHOST:
			f_RPL_YOURHOST(client);
			break;
		case RPL_CREATED:
			f_RPL_CREATED(client);
			break;
		case RPL_MYINFO:
			f_RPL_MYINFO(client);
			break;
		case RPL_ISUPPORT:
			f_RPL_ISUPPORT(client);
			break;
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
	std::string	code = " 381 ";

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

	std::string	str = prefixServer() + code + client.getNickname() + " = " + channel_name
		+ " :";
	Channel *chan = findChannel(channel_name);
	for(int i = 0; i < chan->size(); i++)
	{
		if (i == 0)
			str += "@";
		str += ((*chan)[i].getNickname() + " ");
	}
	str += "\n";
	client.sendToClient(str);
}

void	Server::f_RPL_ENDOFNAMES(Client &client, const std::string& channel_name)
{
	std::string					code = " 366 ";

	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name
		+ " :END of /NAMES list\n";
	client.sendToClient(str);
}

void	Server::f_RPL_KILLREPLY(Client &client, std::string cible_nick, Client& killer, std::string &comment)
{
	std::string code = " 1001 ";
	std::string	str = prefixServer() + code + cible_nick + " was KILLED  by "
		+ killer.getNickname() + " " + comment + "\n";
	client.sendToClient(str);
}

void	Server::f_RPL_WELCOME(Client &client)
{
	std::string code = " 001 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " :Welcome to the Internet Relay Network, " + client.getNickname() + "\n";
	client.sendToClient(str);
}

void	Server::f_RPL_YOURHOST(Client &client)
{
	std::string code = " 002 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " :Your host is " + _name + ", running version 1.0.42 \n";
	client.sendToClient(str);
}

void	Server::f_RPL_CREATED(Client &client)
{
	std::string code = " 003 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " :This server was created Mon Feb 20 2023 at 16:56:42 \n";
	client.sendToClient(str);
}

void	Server::f_RPL_MYINFO(Client &client)
{
	std::string code = " 004 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " " + _name + " 1.0.42 " + "\'no user modes available\'\n";
	client.sendToClient(str);
}

void	Server::f_RPL_ISUPPORT(Client &client)
{
	//fonction a faire a la fin pou rajouter tt les commandes et option de commandes
	// si + de 13 token faire plusieurs sendToClient

	std::string code = " 005 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " NICK PING :are supported by this server\n";
	client.sendToClient(str);
}
