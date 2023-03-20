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
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("KILL"), &Server::cmd_KILL));

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
	else if (checkValidName(args[0]) == false)
		error_handler(ERR_ERRONEUSNICKNAME, client);
	else
		client.setNickname(args[0]);
	std::cout << client.getNickname() <<std::endl;
}

void	Server::cmd_PING(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PING");

	if (args.size() != 1)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
		client.sendToClient(prefixServer() + " PONG " + _name + " :" + args[0] + "\n");
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
	//client.check registration
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
	std::vector<std::string>	args = findArgsCmd(cmd, "KILL");
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

void	Server::error_handler(int ERR_CODE, Client &client)
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
	std::vector<std::string> args = findArgsCmd(client.getCmd(), "KILL");
	std::string str = prefixServer() + code + client.getNickname() + " " + args[0] + " "
		+ ":No such nick\n";
	client.sendToClient(str);
}



/* --------------------------------------------------------------------------------- */


/* RPL */


void	Server::reply_handler(int RPL_CODE, Client &client)
{
	switch (RPL_CODE)
	{
		case RPL_YOUREOPER:
			f_RPL_YOUREOPER(client);
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

void	Server::f_RPL_KILLREPLY(Client &client, std::string cible_nick, Client& killer, std::string &comment)
{
	std::string code = " 1001 ";
	std::string	str = prefixServer() + code + cible_nick + " was KILLED  by "
		+ killer.getNickname() + " " + comment + "\n";
	client.sendToClient(str);
}
