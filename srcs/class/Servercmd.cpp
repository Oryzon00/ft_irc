#include "Server.hpp"

/* CMD */

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
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("QUIT"), &Server::cmd_QUIT));
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
		error_handler(ERR_NEEDMOREPARAMS, client);
	else if (client.getPassOk())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args[0] != _password)
		error_handler(ERR_PASSWDMISMATCH, client);
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
		error_handler(ERR_NEEDMOREPARAMS, client);
	else if (checkAvailNick(args[0]) == false)
		error_handler(ERR_NICKNAMEINUSE, client);
	else if (checkValidName(args[0]) == false)
		error_handler(ERR_ERRONEUSNICKNAME, client);
	else
		client.setNickname(args[0]);
	std::cout << client.getNickname() <<std::endl;
	client.clearCmd();
}

void							Server::cmd_PING(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PING");

	if (args.size() != 1)
		error_handler(ERR_NEEDMOREPARAMS, client);
	else
		client.sendToClient(prefixServer() + " PONG " + _name + " :" + args[0] + "\n");
	client.clearCmd();
}

void							Server::cmd_QUIT(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "QUIT");

	std::string	str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " QUIT ";
	if (!args.empty())
		str += args[0];
	else
		str += ":";

	std::cout << str << std::endl; // A envoyer a tout les users
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
		error_handler(ERR_NEEDMOREPARAMS, client);
	else
	{
		client.setUsername(args[0]);
		client.setRealname(args[3].substr(1, std::string::npos));
		client.setRegistered(true);
		welcomeClient(client);
	}
	client.clearCmd();
}





/* --------------------------------------------------------------------------------- */

/* ERR */

void	Server::error_handler(int ERR_CODE, Client &client)
{
	switch (ERR_CODE)
	{
		case ERR_NEEDMOREPARAMS:
			f_ERR_NEEDMOREPARAMS(client);
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
		default:
			break;
	}
}

void	Server::f_ERR_NEEDMOREPARAMS(Client &client)
{
	std::string code = " 461 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Not Enough Parameters\n";
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

void							Server::f_ERR_NOMOTD(Client &client)
{
	std::string code = " 422 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :MOTD File is missing\n";
	client.sendToClient(str);
}

/* --------------------------------------------------------------------------------- */


/* RPL */


void	Server::reply_handler(int RPL_CODE, Client &client)
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
		default:
			break;
	}
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