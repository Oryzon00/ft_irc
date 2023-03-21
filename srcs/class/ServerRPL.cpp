#include "Server.hpp"

#include "../../includes/Server.hpp"

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
		case RPL_UMODEIS:
			f_RPL_UMODEIS(client);
			break;
		case RPL_TOPIC:
			f_RPL_TOPIC(client, str);
		default:
			break;
	}
}


void	Server::f_RPL_UMODEIS(Client &client)
{
	std::string	code = " 221 ";
	std::string modes = ":";
	if (client.getModeI())
		modes += "i";
	if (client.getModeO())
		modes += "O";
	if (client.getModeR())
		modes += "r";

	std::string	str = prefixServer() + code + client.getNickname() + " "
		+ modes + "\n";
	client.sendToClient(str);
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
	std::string	str = prefixServer() + code + cible_nick + " was KILLED by "
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

void	Server::f_RPL_NOTOPIC(Client &client, std::string channel_name)
{
	std::string	code = " 331 ";

	std::string str = prefixServer() + code + client.getNickname() + " " channel_name + " :No topic is set\n";
	client.sendToClient(str);
}
