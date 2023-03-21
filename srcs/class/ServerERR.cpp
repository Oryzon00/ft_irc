#include "Server.hpp"

/* ERR */

void	Server::error_handler(int ERR_CODE, Client &client, const std::string& str)
{
	switch (ERR_CODE)
	{
		case ERR_WRONGNBPARAMS:
			f_ERR_WRONGNBPARAMS(client);
			break;
		case ERR_ALREADYREGISTERED:
			f_ERR_ALREADYREGISTERED(client);
			break;
		case ERR_PASSWDMISMATCH:
			f_ERR_PASSWDMISMATCH(client);
			break;
		case ERR_NICKNAMEINUSE:
			f_ERR_NICKNAMEINUSE(client);
			break;
		case ERR_ERRONEUSNICKNAME:
			f_ERR_ERRONEUSNICKNAME(client);
			break;
		case ERR_NOMOTD:
			f_ERR_NOMOTD(client);
			break;
		case ERR_NOOPERHOST:
			f_ERR_NOOPERHOST(client);
			break;
		case ERR_NOPRIVILEGES:
			f_ERR_NOPRIVILEGES(client);
			break;
		case ERR_NOSUCHNICK:
			f_ERR_NOSUCHNICK(client);
			break;
		case ERR_NOTREGISTERED:
			f_ERR_NOTREGISTERED(client);
			break;
		case ERR_USERSDONTMATCH:
			f_ERR_USERSDONTMATCH(client);
			break;
		case ERR_BADCHANNELKEY:
			f_ERR_BADCHANNELKEY(client, str);
			break;
		case ERR_BADCHANMASK:
			f_ERR_BADCHANMASK(client, str);
			break;
		case ERR_BANNEDFROMCHAN:
			f_ERR_BANNEDFROMCHAN(client, str);
			break;
		case ERR_INVITEONLYCHAN:
			f_ERR_INVITEONLYCHAN(client, str);
			break;
		case ERR_NOTONCHANNEL:
			f_ERR_NOTONCHANNEL(client, str);
			break;
		case ERR_NOSUCHCHANNEL:
			f_ERR_NOSUCHCHANNEL(client, str);
			break;
		default:
			break;
	}
}

void	Server::f_ERR_USERSDONTMATCH(Client &client)
{
	std::string code = " 502 ";
	std::string str = prefixServer() + code + client.getNickname() + " "
		+ ":Cant change mode for other users\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOTREGISTERED(Client &client)
{
	std::string code = " 1002 ";
	std::string str = prefixServer() + code + client.getNickname() + " "
		+ ": You are not registered.\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOOPERHOST(Client &client)
{
	std::string code = " 491 ";
	std::string	str = prefixServer() + code + client.getNickname() + " "
		+ " :No O-lines for your host\n";
    client.sendToClient(str);
}

void	Server::f_ERR_WRONGNBPARAMS(Client &client)
{
	std::string code = " 461 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Wrong number of parameters\n";
    client.sendToClient(str);
}

void	Server::f_ERR_ALREADYREGISTERED(Client &client)
{
	std::string code = " 462 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :You may not reregister\n";
    client.sendToClient(str);
}

void	Server::f_ERR_PASSWDMISMATCH(Client &client)
{
	std::string code = " 464 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :Password incorrect\n";
    client.sendToClient(str);
}

void	Server::f_ERR_UNKNOWNCOMMAND(Client &client)
{
	std::string code = " 421 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findKey(client.getCmd())
		+ " :Unknown command\n";
    client.sendToClient(str);
	client.clearCmd();
}

void	Server::f_ERR_NICKNAMEINUSE(Client &client)
{
	std::string code = " 433 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + findArgsCmd(client.getCmd(), "NICK")[0] 
		+ " :Nickname is already in use\n";
    client.sendToClient(str);
}

void	Server::f_ERR_ERRONEUSNICKNAME(Client &client)
{
	std::string code = " 432 ";
	std::vector<std::string> args = findArgsCmd(client.getCmd(), "NICK");
	std::string str;
	if (args.size() == 1)
		str = prefixServer() + code + client.getNickname() + " " + args[0] 
			+ " :Erroneus nickname\n";
    else
		str = prefixServer() + code + client.getNickname() + args[0] + " " + args[1]
			+ " :Erroneus nickname\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOMOTD(Client &client)
{
	std::string code = " 422 ";
	std::string	str = prefixServer() + code + client.getNickname() + " :MOTD is not supported\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOPRIVILEGES(Client &client)
{
	std::string code = " 481 ";
	std::string str = prefixServer() + code + client.getNickname() + " " 
		+ ":Permission Denied - You're not an IRC operator\n";
	client.sendToClient(str);
}

void	Server::f_ERR_NOSUCHNICK(Client & client)
{
	std::string code = " 401 ";
	std::vector<std::string> args = findArgsCmd(client.getCmd(), "kill");
	std::string str = prefixServer() + code + client.getNickname() + " " + args[0] + " "
		+ ":No such nick\n";
	client.sendToClient(str);
}

void							Server::f_ERR_BADCHANMASK(Client &client, const std::string& channel_name)
{
	std::string code = " 476 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Bad Channel Mask\n";
    client.sendToClient(str);
}


void							Server::f_ERR_BADCHANNELKEY(Client &client, const std::string& channel_name)
{
	std::string code = " 475 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Cannot join channel (Wrong Key)\n";
    client.sendToClient(str);
}

void							Server::f_ERR_BANNEDFROMCHAN(Client &client, const std::string& channel_name)
{
	std::string code = " 474 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :You are banned from this channel, creep (+b)\n";
    client.sendToClient(str);
}

void							Server::f_ERR_INVITEONLYCHAN(Client &client, const std::string& channel_name)
{
	std::string code = " 473 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :Cannot join channel (Invite ONLY Channel +i)\n";
    client.sendToClient(str);
}

void							Server::f_ERR_NOTONCHANNEL(Client &client, const std::string& channel_name)
{
	std::string code = " 442 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " : You're not on that channel\n";
    client.sendToClient(str);
}

void							Server::f_ERR_NOSUCHCHANNEL(Client &client, const std::string& channel_name)
{
	std::string code = " 403 ";
	std::string	str = prefixServer() + code + client.getNickname() + " " + channel_name + " :No such channel\n";
    client.sendToClient(str);
}

/* --------------------------------------------------------------------------------- */
