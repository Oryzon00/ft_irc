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
		error_handler(462, client);
	else if (args[0] != _password)
		error_handler(464, client);
	else
		client.setPassOk(true);
	client.clearCmd();

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
		default:
			break;
	}
}

void							Server::f_ERR_NEEDMOREPARAMS(Client &client)
{
	std::string code = " 461 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ ": Not Enough Parameters";
    client.sendToClient(str);
	quitClientCmd(client);
}

/* --------------------------------------------------------------------------------- */


/* RPL */


void							Server::reply_handler(int RPL_CODE, Client &client)
{
	(void) RPL_CODE;
	(void) client;
}
