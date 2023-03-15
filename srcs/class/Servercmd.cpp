#include "Server.hpp"


/* CMD */

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
