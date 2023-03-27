#include "Server.hpp"

void	Server::BotProcess(Client& client, const std::string& message)
{
	(void) client;
	(void) message;
	std::cout << "Bot detected" << std::endl;
}
