#include "Server.hpp"

void	Server::BotCmdHelp(Client& client, Channel* channel)
{
	(void)client;
	(void)channel;
}

void	Server::BotCmdUnknown(Client& client, Channel* channel)
{
	(void)client;
	(void)channel;
}

void	Server::BotCmdUsers(Channel *channel)
{
	std::string users;

	for (size_t i = 1; i != _clients.size(); i++)
		users += _clients[i].getNickname() + " ";
	std::string str = ":" + _bot.getNickname() + "!~" + _bot.getUsername() + "@" + _name + " NOTICE #bot " + users + "\n";
	channel->SendToAll(_bot, str);
}

void	Server::BotProcess(Client& client, const std::string& message)
{
	Channel *channel = findChannel("#bot");
	std::cout << "Bot detected, command : " << message << std::endl;
	if (message == "!help")
		BotCmdHelp(client, channel);
	else if (message == ":!users")
		BotCmdUsers(channel);
	else
		BotCmdUnknown(client, channel);

}
