#include "Server.hpp"

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
		case RPL_NOTOPIC:
			f_RPL_NOTOPIC(client, str);
			break;
		case RPL_MOTDSTART:
			f_RPL_MOTDSTART(client);
			break;
		case RPL_MOTD:
			f_RPL_MOTD(client);
			break;
		case RPL_ENDOFMOTD:
			f_RPL_ENDOFMOTD(client);
			break;
		case RPL_LISTSTART:
			f_RPL_LISTSTART(client);
			break;
		case RPL_LIST:
			f_RPL_LIST(client, str);
			break;
		case RPL_LISTEND:
			f_RPL_LISTEND(client);
			break;
		case RPL_INVITING:
			f_RPL_INVITING(client, str);
			break;
		default:
			break;
	}
}

void	Server::f_RPL_CHANNELMODEIS(Client& client, Channel& channel)
{
	std::string code = " 324 ";
	std::string	modes = ":";
	if (channel.getModeI())
		modes += "i";
	if (channel.getKey() != "")
		modes += "k";
	if (channel.getModeM())
		modes += "m";
	if (channel.getModeS())
		modes += "s";
	if(channel.getModeT())
		modes += "t";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel.getName() + " "
		+ modes + "\n";
	client.sendToClient(str);
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

/*
001 ad :Welcome to the Internet Relay Network ad!ajung@localhost!
375 ad!ajung@localhost :- NormIrc Message of the day -
*/

/*
:13.20h 001 quentin :Welcome to the Internet Relay Network, quentin
:13.20h 375 quentin :-13.20h Message of the day -
*/

void	Server::f_RPL_WELCOME(Client &client)
{
	std::string code = " 001 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " :Welcome to the Internet Relay Network " 
		+ client.getNickname() + "!" + client.getUsername() + "@" + _name + "\n";
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

	str = prefixServer() + code + client.getNickname() + " " + _name + " 1.0.42 " + "iOR imstk\n";
	client.sendToClient(str);
}
void	Server::f_RPL_ISUPPORT(Client &client)
{
	//fonction a faire a la fin pou rajouter tt les commandes et option de commandes
	// si + de 13 token faire plusieurs sendToClient

	std::string code = " 005 ";
	std::string str;

	str = prefixServer() + code + client.getNickname() + " CHANMODES=#@ NICKLEN=20 :are supported by this server\n";
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
	std::string 				channel_type = " = ";


	Channel *chan = findChannel(channel_name);
	if (chan->getModeS())
		channel_type = " @ ";
	std::string	str = prefixServer() + code + client.getNickname() + channel_type + channel_name
						 + " :";
	for (int i = 0; i < chan->size(); i++)
	{
		Client* tmp = find_client_by_id((*chan)[i]);
		if (tmp && (chan->isMember(client) || !tmp->getModeI()))
		{
			if (i == 0)
				str += "@";
			str += (tmp->getNickname() + " ");
		}
		else if ((*chan)[i] == 0)
			str += "@Mr.Bot ";
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

	std::string str = prefixServer() + code + client.getNickname() + " " + channel_name + " :No topic is set\n";
	client.sendToClient(str);
}

void	Server::f_RPL_MOTDSTART(Client &client)
{
	std::string code = " 375 ";

	std::string str = prefixServer() + code + client.getNickname() + "!" + client.getUsername() + "@" + _name 
		+ " :-" + _name + " Message of the day - \n";
	client.sendToClient(str);
}

void	Server::f_RPL_MOTD(Client &client)
{
	std::string code = " 372 ";

	std::string str = prefixServer() + code + client.getNickname() + " :" + _MOTD + "\n";
	client.sendToClient(str);
}

void	Server::f_RPL_ENDOFMOTD(Client &client)
{
	std::string code = " 376 ";

	std::string str = prefixServer() + code + client.getNickname() + " :End of /MOTD command.\n";
	client.sendToClient(str);
}

void	Server::f_RPL_LISTSTART(Client &client)
{
	std::string code = " 321 ";

	std::string str = prefixServer() + code + client.getNickname() + " Channel :Users Name\n";
	client.sendToClient(str);
}

void	Server::f_RPL_LIST(Client &client, std::string channel_name)
{
	std::string code = " 322 ";

	Channel *channel = findChannel(channel_name);
	if (channel && !channel->getModeS())
	{
		std::string result = static_cast<std::ostringstream*>( &(std::ostringstream() << channel->size()) )->str();
		std::string str = prefixServer() + code + client.getNickname() + " " + channel_name + " " + result + " :" + channel->getTopic() + "\n";
		client.sendToClient(str);
	}
}

void	Server::f_RPL_LISTEND(Client &client)
{
	std::string code = " 323 ";
	std::string str = prefixServer() + code + client.getNickname() + " :End of /LIST\n";
	client.sendToClient(str);
}

void	Server::f_RPL_INVITING(Client &client, std::string s)
{
	std::string	code = " 341 ";

	std::string str = prefixServer() + code + client.getNickname() + " " + s + "\n";
	client.sendToClient(str);
}
