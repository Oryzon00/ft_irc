#include "Server.hpp"

void	Server::BotCmdHelp(Client& client, Channel* channel)
{
	
}

void	Server::BotCmdUnknown(Client& client, Channel* channel)
{

}


void	Server::BotProcess(Client& client, const std::string& message)
{
	(void) client;
	(void) message;
	Channel *channel = findChannel("#bot");
	std::cout << "Bot detected" << std::endl;
	if (message == "!help")
		BotCmdHelp(client, channel);
	else
		BotCmdUnknown(client, channel);

}
