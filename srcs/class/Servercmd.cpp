#include "Server.hpp"

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
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("PART"), &Server::cmd_PART));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("JOIN"), &Server::cmd_JOIN));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("QUIT"), &Server::cmd_QUIT));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("OPER"), &Server::cmd_OPER));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("MODE"), &Server::cmd_MODE));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("kill"), &Server::cmd_KILL));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("restart"), &Server::cmd_RESTART));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("PRIVMSG"), &Server::cmd_PRIVMSG));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("TOPIC"), &Server::cmd_TOPIC));
	_dico.insert(std::pair<std::string, cmdFunction>(std::string("INVITE"), &Server::cmd_INVITE));
}

/* --------------------------------------------------------------------------------- */

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

/* CMD */

void	Server::cmd_RESTART(std::string& cmd, Client& client)
{
	(void) cmd;
	if (!client.getModeO())
		error_handler(ERR_NOPRIVILEGES, client);
	else
		throw RestartException();
}

void	Server::cmd_CAP(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "CAP");
	if (args.size() != 1 || args[0] != "LS")
	{
		removeClient(client);
		std::cout << "!! -- Client CAP is not LS -- !!" << std::endl;
	}
	client.setIsIrssi(true);
}

void	Server::cmd_PASS(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PASS");
	if (args.empty())
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (client.getPassOk())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args[0] != _password)
	{
		error_handler(ERR_PASSWDMISMATCH, client);
		quitClientCmd(client);
	}
	else
		client.setPassOk(true);

}

void	Server::cmd_NICK(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "NICK");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (args.size() != 1)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (checkAvailNick(args[0]) == false)
		error_handler(ERR_NICKNAMEINUSE, client);
	else if (checkValidNick(args[0]) == false)
		error_handler(ERR_ERRONEUSNICKNAME, client);
	else
	{
		std::string	str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name
							+ " NICK " + args[0] + "\n";
		client.sendToClient(str);
		client.setNickname(args[0]);
	}
}

void	Server::cmd_PING(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PING");

	if (args.size() != 1)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
		client.sendToClient(prefixServer() + " PONG " + _name + " :" + args[0] + "\n");
}

void	Server::cmd_QUIT(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "QUIT");

	std::string	str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " QUIT ";
	if (!args.empty())
		str += args[0] + "\n";
	else
		str += ":left without a reason :o\n";

	for (std::vector<Client>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
		it->sendToClient(str);
	quitClientCmd(client);
}

void	Server::cmd_USER(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "USER");
	if (!client.getPassOk())
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (client.getModeR())
		error_handler(ERR_ALREADYREGISTERED, client);
	else if (args.size() != 4)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		client.setUsername(args[0]);
		client.setRealname(args[3].substr(1, std::string::npos));
		client.setModeR(true);
		welcomeClient(client);
	}
	client.clearCmd();
}

void	Server::join_channel(Client& client, std::string name, std::string key)
{
	Channel* channel = findChannel(name);
	if (!channel && validChannelName(name))
	{
		_chans.push_back(Channel(&client, name, key));
		client.sendToClient(":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " JOIN :" + name + "\n");
		reply_handler(RPL_TOPIC, client, name);
		reply_handler(RPL_NAMREPLY, client, name);
		reply_handler(RPL_ENDOFNAMES, client, name);
	}
	else if (!channel)
		error_handler(ERR_BADCHANMASK, client, name);
	else if (key != channel->getKey())
		error_handler(ERR_BADCHANNELKEY, client, name);
	else if (channel->isBanned(client))
		error_handler(ERR_BANNEDFROMCHAN, client, name);
	else if (!channel->isInvited(client))
		error_handler(ERR_INVITEONLYCHAN, client, name);
	else if (!channel->isMember(client))
	{
		channel->addMember(&client);
		channel->SendToAll(client, ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " JOIN :" + name + "\n");
		reply_handler(RPL_TOPIC, client, name);
		reply_handler(RPL_NAMREPLY, client, name);
		reply_handler(RPL_ENDOFNAMES, client, name);
	}
}

void	Server::cmd_JOIN(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "JOIN");
	if (args.size() <= 2 && !args.empty())
	{
		std::vector<std::string>		chans = strToVec(args[0], ",");
		std::vector<std::string>		keys;
		if (args.begin() + 1 != args.end())
			keys = strToVec(args[1], ",");
		for(size_t i = 0; i < chans.size(); i++)
			join_channel(client, chans[i], (i < keys.size()) ? keys[i] : "x");
	}
	else
		error_handler(ERR_WRONGNBPARAMS, client);
}

void	Server::part_channel(Client& client, std::string name, std::string reason)
{
	Channel* channel = findChannel(name);
	if (!channel)
		error_handler(ERR_NOSUCHCHANNEL, client, name);
	else if (!channel->isMember(client))
		error_handler(ERR_NOTONCHANNEL, client, name);
	else
	{
		std::string str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name 
							+ " PART " + name;
		if (!reason.empty())
			str += " " + reason;
		str += "\r\n";
		client.sendToClient(str);
		channel->SendToAll(client, str);
		channel->removeMember(client);
		if (!channel->size())
			removeChannel(*channel);
	}
}

//	:b!~qcherel@my.server.name PART #chat
//	:b!~qcherel@13-20h.IRC PART #c

void	Server::cmd_PART(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PART");
	if (args.size() <= 2 && !args.empty())
	{
		std::vector<std::string>		chans = strToVec(args[0], ",");
		std::string						reason;
		if (args.begin() + 1 != args.end())
			reason = args[1];
		for(size_t i = 0; i < chans.size(); i++)
			part_channel(client, chans[i], reason);
	}
	else
		error_handler(ERR_WRONGNBPARAMS, client);
}

void							Server::cmd_INVITE(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "INVITE");
	if (args.size() != 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		Channel*	channel = findChannel(args[1]);
		Client*		target  = find_client_by_nick(args[0]);
		if (!channel)
			error_handler(ERR_NOSUCHCHANNEL, client, args[1]);
		else if (!target)
			error_handler(ERR_NOSUCHNICK, client, args[0]);
		else if (!channel->isMember(client))
			error_handler(ERR_NOTONCHANNEL, client, args[1]);
		else if (!checkOP(client, *channel))
			error_handler(ERR_CHANOPRIVSNEEDED, client, args[1]);
		else if (channel->isMember(*target))
			error_handler(ERR_USERONCHANNEL, client, args[0] + " " + args[1]);
		else
		{
			std::string str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name 
							+ " INVITE " + args[0] + " :" + channel->getName() + "\n";
			reply_handler(RPL_INVITING, client,
						target->getNickname() + " " + channel->getName());
			target->sendToClient(str);
			channel->invite(target);
		}	
	}
}


void	Server::cmd_OPER(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "OPER");
	Client*	client_oper = NULL;

	if (args.size() == 2)
		client_oper = find_client_by_nick(args[0]);
	if (args.size() != 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (args[1] != OPER_PASSWD)
		error_handler(ERR_PASSWDMISMATCH, client);
	else if (client_oper == NULL)
		error_handler(ERR_NOOPERHOST, client);
	else
	{
		reply_handler(RPL_YOUREOPER, *client_oper);
		client_oper->setModeO(true);
	}
}

void	Server::cmd_KILL(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "kill");
	Client*						client_cible = NULL;
	std::string					cible_nick;
	std::string					comment;

	if (args.size() == 2)
	{
		cible_nick = args[0];
		comment = args[1];
		client_cible = find_client_by_nick(cible_nick);
	}

	if (args.size() != 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (!client.getModeO())
		error_handler(ERR_NOPRIVILEGES, client);
	else if (!client_cible)
		error_handler(ERR_NOSUCHNICK, client, "kill");
	else
	{
		for (std::vector<Client>::iterator it = _clients.begin() + 1; it != _clients.end(); it++)
			f_RPL_KILLREPLY(*it, cible_nick, client, comment);
		if (client_cible)
		{
			client.clearCmd();
			quitClientCmd(*client_cible);
		}
	}
}
void	Server::message_to_client(std::string clientTargetName, Client &client, std::string message)
{
	Client *target = find_client_by_nick(clientTargetName);
	std::string str;

	if (!target)
		error_handler(ERR_NOSUCHNICK, client);
	else
	{
		str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " PRIVMSG "
				+ clientTargetName + " " + message + "\n";
		target->sendToClient(str);
	}
}

void	Server::message_to_channel(std::string channelTargetName, Client &client, std::string message)
{
	Channel *target = findChannel(channelTargetName);
	std::string str;

	if (!target || !target->isMember(client) || target->getModeM())
		error_handler(ERR_CANNOTSENDTOCHAN, client);
	else
	{
		str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name + " PRIVMSG "
			  + channelTargetName + " " + message + "\n";
		target->SendToAll(client, str);
	}
}

void	Server::cmd_PRIVMSG(std::string& cmd, Client& client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "PRIVMSG");

	if (args.size() != 2 || args[1].at(0) != ':')
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		std::vector <std::string> targets = strToVec(args[0], ",");
		for (size_t i = 0; i < targets.size(); i++) {
			if (targets[i].at(0) == '#')
			{
				message_to_channel(targets[i], client, args[1]);
				std::cout << "channel : '" << targets[i] << "' with message : " << args[1] << "'" << std::endl;
			}
			else
			{
				message_to_client(targets[i], client, args[1]);
				std::cout << "user : '" << targets[i] << "' with message : '" << args[1] << "'" << std::endl;
			}
		}
	}
}

void	Server::cmd_TOPIC(std::string &cmd, Client &client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "TOPIC");
	Channel* target;

	if (args.empty() || args.size() > 2)
		error_handler(ERR_WRONGNBPARAMS, client);
	else
	{
		target = findChannel(args[0]);
		if (!target)
			error_handler(ERR_NOSUCHCHANNEL, client, args[0]);
		else if (!target->isMember(client))
			error_handler(ERR_NOTONCHANNEL, client, args[0]);
		else if (args.size() == 2)
		{
			if (target->getModeT() && !target->isChanOp(client) && !client.getModeO())
				error_handler(ERR_CHANOPRIVSNEEDED, client, args[0]);
			else
			{
				std::string str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name
										  + " TOPIC " + args[0] + " " + args[1] + "\n";
				target->setTopic(args[1]);
				client.sendToClient(str);
				target->SendToAll(client, str);

			}
		}
		else
		{
			if (target->getTopic().empty())
				reply_handler(RPL_NOTOPIC, client, args[0]);
			else
				reply_handler(RPL_TOPIC, client, args[0]);
		}
	}
}
