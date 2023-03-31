# include "Server.hpp"

void	Server::cmd_MODE(std::string& cmd, Client & client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "MODE");
	if (args.size() == 0)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (args[0][0] != '#')
		cmd_MODE_user(cmd, client, args);
	else if (args[0][0] == '#')
		cmd_MODE_channel(cmd, client, args);
}

/* --------------------------------------------------------------------------------- */

void	Server::cmd_MODE_user(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	std::string	nick = args[0];
	Client*		target = find_client_by_nick(nick);

	if (!target)
		error_handler(ERR_NOSUCHNICK, client, "MODE");
	else if (nick != client.getNickname())
		error_handler(ERR_USERSDONTMATCH, client);
	else if (args.size() == 1)
		reply_handler(RPL_UMODEIS, client);
	else
	{
		if (args[1][0] == '+')
			cmd_MODE_user_add(cmd, client, args);
		else if (args[1][0] == '-')
			cmd_MODE_user_remove(cmd, client, args);
		else
			error_handler(ERR_UMODEUNKNOWNFLAG, client);
	}
}

void	Server::cmd_MODE_user_add(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void) cmd;
	std::string target = args[0];
	std::string	flag = args[1];
	std::string::iterator it = flag.begin();
	it++;
	for(; it != flag.end(); it++)
	{
		if (*it == 'i')
		{
			client.setModeI(true);
			cmd_MODE_answer(client, target, "+i");
		}
		else if (*it == 'O')
			error_handler(ERR_NORIGHT, client);
		else if (*it == 'r')
			error_handler(ERR_NORIGHT, client);
		else
			error_handler(ERR_UMODEUNKNOWNFLAG, client);
	}
}

void	Server::cmd_MODE_user_remove(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void) cmd;
	std::string target = args[0];
	std::string	flag = args[1];
	std::string::iterator it = flag.begin();
	it++;
	for(; it != flag.end(); it++)
	{
		if (*it == 'i')
		{
			client.setModeI(false);
			cmd_MODE_answer(client, target, "-i");
		}
		else if (*it == 'O')
			error_handler(ERR_NORIGHT, client);
		else if (*it == 'r')
			error_handler(ERR_NORIGHT, client);
		else
			error_handler(ERR_UMODEUNKNOWNFLAG, client);
	}
}

/* --------------------------------------------------------------------------------- */




void	Server::cmd_MODE_channel(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	std::string		channel = args[0];
	Channel*		target = findChannel(channel);

	if (!target)
		error_handler(ERR_NOSUCHCHANNEL, client, channel);
	else if (args.size() == 1)
		f_RPL_CHANNELMODEIS(client, *target);
	else
		cmd_MODE_channel_parse(cmd, client, args);
}

void	Server::cmd_MODE_channel_parse(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void) cmd;
	char					sign = SIGN_NONE;
	std::string 			channel_name = args[0];
	Channel					*channel = findChannel(channel_name);
	std::string				flag = args[1];
	std::string::iterator	it = flag.begin();

	for(; it != flag.end(); it++)
	{
		if (*it == '+')
			sign = SIGN_PLUS;
		else if (*it == '-')
			sign = SIGN_MINUS;
		else if (sign == SIGN_NONE)
			;
		else if (*it == 'i')
			cmd_MODE_channel_i(client, channel, channel_name, sign, *it);
		else if (*it == 'm')
			cmd_MODE_channel_m(client, channel, channel_name, sign, *it);
		else if (*it == 's')
			cmd_MODE_channel_s(client, channel, channel_name, sign, *it);
		else if (*it == 't')
			cmd_MODE_channel_t(client, channel, channel_name, sign, *it);
		else if (*it == 'k')
			error_handler(ERR_NORIGHT, client);
		else
			error_handler(ERR_UNKNOWNMODE, client, std::string(1, *it));
	}
}

void	Server::cmd_MODE_channel_i(Client& client, Channel* channel,
								std::string& channel_name, char& sign, char mode)
{
	if (sign == SIGN_NONE)
		error_handler(ERR_UNKNOWNMODE, client, std::string(1, mode));
	else if (!checkOP(client, *channel))
		error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name); 
	else if (sign == SIGN_PLUS)
	{
		channel->setModeI(true);
		cmd_MODE_answer_channel(client, channel_name, "+i", channel);
	}
	else if (sign == SIGN_MINUS)
	{
		channel->setModeI(false);
		cmd_MODE_answer_channel(client, channel_name, "-i", channel);
	}
}

void	Server::cmd_MODE_channel_m(Client& client, Channel* channel,
								std::string& channel_name, char& sign, char mode)
{
	if (sign == SIGN_NONE)
		error_handler(ERR_UNKNOWNMODE, client, std::string(1, mode));
	else if (!checkOP(client, *channel))
		error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name); 
	else if (sign == SIGN_PLUS)
	{
		channel->setModeM(true);
		cmd_MODE_answer_channel(client, channel_name, "+m", channel);
	}
	else if (sign == SIGN_MINUS)
	{
		channel->setModeM(false);
		cmd_MODE_answer_channel(client, channel_name, "-m", channel);
	}
}

void	Server::cmd_MODE_channel_s(Client& client, Channel* channel,
								std::string& channel_name, char& sign, char mode)
{
	if (sign == SIGN_NONE)
		error_handler(ERR_UNKNOWNMODE, client, std::string(1, mode));
	else if (!checkOP(client, *channel))
		error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name); 
	else if (sign == SIGN_PLUS)
	{
		channel->setModeS(true);
		cmd_MODE_answer_channel(client, channel_name, "+s", channel);
	}
	else if (sign == SIGN_MINUS)
	{
		channel->setModeS(false);
		cmd_MODE_answer_channel(client, channel_name, "-s", channel);
	}
}

void	Server::cmd_MODE_channel_t(Client& client, Channel* channel,
								std::string& channel_name, char& sign, char mode)
{
	if (sign == SIGN_NONE)
		error_handler(ERR_UNKNOWNMODE, client, std::string(1, mode));
	else if (!checkOP(client, *channel))
		error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name); 
	else if (sign == SIGN_PLUS)
	{
		channel->setModeT(true);
		cmd_MODE_answer_channel(client, channel_name, "+t", channel);
	}
	else if (sign == SIGN_MINUS)
	{
		channel->setModeT(false);
		cmd_MODE_answer_channel(client, channel_name, "-t", channel);
	}
}

void	Server::cmd_MODE_answer(Client & client, std::string& target, std::string flag)
{
	std::string str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name +
		+ " MODE " + target + " :" + flag + "\n";
	client.sendToClient(str);
}


void	Server::cmd_MODE_answer_channel(Client & client, std::string& target, std::string flag,
										Channel* channel)
{
	std::string str = ":" + client.getNickname() + "!~" + client.getUsername() + "@" + _name +
		+ " MODE " + target + " " + flag + "\n";
	client.sendToClient(str);
	sendToChannel(channel, client, str);
}

/* void	Server::cmd_MODE_channel(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	std::string		channel = args[0];
	Channel*		target = findChannel(channel);

	if (!target)
		error_handler(ERR_NOSUCHCHANNEL, client, channel);
	else if (args.size() == 1)
		f_RPL_CHANNELMODEIS(client, *target);
	else
	{
		if (args[1][0] == '+')
			cmd_MODE_channel_add(cmd, client, args);
		else if (args[1][0] == '-')
			cmd_MODE_channel_remove(cmd, client, args);
		else
			error_handler(ERR_UNKNOWNMODE, client, args[1]);
	}
} */

/* void	Server::cmd_MODE_channel_remove(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void) cmd;
	std::string 			channel_name = args[0];
	Channel					*channel = findChannel(channel_name);
	std::string				flag = args[1];
	std::string::iterator	it = flag.begin();
	it++;
	for(; it != flag.end(); it++)
	{
		if (*it == 'i')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeI(false);
				cmd_MODE_answer_channel(client, channel_name, "-i");
			}
		}
		else if (*it == 'm')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeM(false);
				cmd_MODE_answer_channel(client, channel_name, "-m");
			}
		}
		else if (*it == 's')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeS(false);
				cmd_MODE_answer_channel(client, channel_name, "-s");
			}
		}
		else if (*it == 't')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeT(false);
				cmd_MODE_answer_channel(client, channel_name, "-t");
			}
		}
		else if (*it == 'k')
			error_handler(ERR_NORIGHT, client);
		else
			error_handler(ERR_UNKNOWNMODE, client, &(*it));
	}
}		 */	

/* void	Server::cmd_MODE_channel_add(std::string& cmd, Client& client,
								std::vector<std::string>& args)
{
	(void) cmd;
	std::string 			channel_name = args[0];
	Channel					*channel = findChannel(channel_name);
	std::string				flag = args[1];
	std::string::iterator	it = flag.begin();
	it++;
	for(; it != flag.end(); it++)
	{
		if (*it == 'i')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeI(true);
				cmd_MODE_answer_channel(client, channel_name, "+i");
			}
		}
		else if (*it == 'm')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeM(true);
				cmd_MODE_answer_channel(client, channel_name, "+m");
			}
		}
		else if (*it == 's')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeS(true);
				cmd_MODE_answer_channel(client, channel_name, "+s");
			}
		}
		else if (*it == 't')
		{
			if (!checkOP(client, *channel))
				error_handler(ERR_CHANOPRIVSNEEDED, client, channel_name);
			else
			{
				channel->setModeT(true);
				cmd_MODE_answer_channel(client, channel_name, "+t");
			}
		}
		else if (*it == 'k')
			error_handler(ERR_NORIGHT, client);
		else
			error_handler(ERR_UNKNOWNMODE, client, &(*it));
	}
} */



/* void	Server::cmd_MODE(std::string& cmd, Client & client)
{
	std::vector<std::string>	args = findArgsCmd(cmd, "MODE");
	if (args.size() == 0)
		error_handler(ERR_WRONGNBPARAMS, client);
	else if (args[0][0] != '#')
		cmd_MODE_user(cmd, client, args);
	else if (args[0][0] == '#')
		cmd_MODE_channel(cmd, client, args);
	
} */
