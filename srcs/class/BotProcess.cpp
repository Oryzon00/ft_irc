#include "Server.hpp"

void	Server::BotCmdHelp(Channel* channel)
{
	std::string str = ":" + _bot.getNickname() + "!~" + _bot.getUsername() + "@" + _name + " PRIVMSG #bot " + 
					":List of commands available:\n"
					+ "!ping\n"
					+ "!help\n";
	channel->SendToAll(_bot, str);
}

void	Server::BotCmdUnknown(Channel* channel)
{
	std::string str = ":" + _bot.getNickname() + "!~" + _bot.getUsername() + "@" + _name + " PRIVMSG #bot " + 
					":Sorry, i did not understand your command. Use !help" + "\n";
	channel->SendToAll(_bot, str);
}

void	Server::BotCmdPing(Channel* channel)
{
	std::string str = ":" + _bot.getNickname() + "!~" + _bot.getUsername() + "@" + _name + " PRIVMSG #bot :pong\n";
	channel->SendToAll(_bot, str);
}



void	Server::BotCmdBunny(Channel* channel)
{
	std::string str = ":" + _bot.getNickname() + "!~" + _bot.getUsername() + "@" + _name + " PRIVMSG #bot :" +  "\n";
	channel->SendToAll(_bot, str);
}


void	Server::BotProcess(Client& client, const std::string& message)
{
	(void) client;
	Channel *channel = findChannel("#bot");
	std::cout << "Bot detected" << std::endl;
	if (message == ":!help")
		BotCmdHelp(channel);
	else if (message == ":!ping")
		BotCmdPing(channel);
	else if (message == ":!bunny")
		BotCmdBunny(channel);
	else
		BotCmdUnknown(channel);

}
