#include "Server.hpp"


/* CMD */

//faire fonction check
//	-passOK
//	-isIrssi

void							Server::quitClientCmd(Client &client)
{
	removeClient(client);
	throw ClientDestroyedException();
}

/* --------------------------------------------------------------------------------- */


void							Server::cmd_CAP(std::string& cmd, Client& client)
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

void							Server::cmd_PASS(std::string& cmd, Client& client)
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

void							Server::cmd_NICK(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PASS");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (args.empty())
		error_handler(ERR_NONICKNAMEGIVEN, client);
	//else if (checkAvailName(args[0]))
}


/* --------------------------------------------------------------------------------- */

/* ERR */

void							Server::error_handler(int ERR_CODE, Client &client)
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
		default:
			break;
	}
}

void							Server::f_ERR_NEEDMOREPARAMS(Client &client)
{
	std::string code = " 461 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Not Enough Parameters\n";
    client.sendToClient(str);
	quitClientCmd(client);
}

void							Server::f_ERR_ALREADYREGISTERED(Client &client)
{
	std::string code = " 462 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :You may not reregister\n";
    client.sendToClient(str);
	quitClientCmd(client);
}

void							Server::f_ERR_PASSWDMISMATCH(Client &client)
{
	std::string code = " 464 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :Password incorrect\n";
    client.sendToClient(str);
	quitClientCmd(client);
}

void							Server::f_ERR_UNKNOWNCOMMAND(Client &client)
{
	std::string code = " 421 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Unknown command\n";
    client.sendToClient(str);
	client.clearCmd();
}


/* --------------------------------------------------------------------------------- */


/* RPL */


void							Server::reply_handler(int RPL_CODE, Client &client)
{
	(void) RPL_CODE;
	(void) client;
}
