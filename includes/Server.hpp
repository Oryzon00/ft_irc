#pragma once

# include <string>
# include <vector>
# include <map>
# include <list>
# include <iostream>
# include <algorithm>
# include <utility>
# include <errno.h>

# include "RPLERRcode.hpp"
# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"
# include "Channel.hpp"
# include "tools.hpp"


/* authenticate, set a nickname, a username, join a channel,
send and receive private messages */

/* commands that are specific to operators */

/*
CMD A FAIRE

CONNECTION
	CAP		DONE
	PASS	DONE
	NICK	DONE
	USER	DONE
	PING	DONE
	OPER	DONE
	QUIT	--> A FINIR (channel)

class CHANNEL --> QUENTIN
check registration --> DONE
001 - 005 + 251 + 255 + 452 --> DONE

Channel Operation
	JOIN message		DONE
	PART message
	TOPIC message			Chan Oper Only if trying to change the topic
	NAMES message
	INVITE message			Chan Oper Only
	KICK message			Chan Oper Only

Sending Messages
	PRIVMSG message --> DONE

Operator Messages
	KILL message	--> DONE
	RESTART message	--> DONE

Server Queries and Commands
	MODE
	MOTD

TO DO
--> merge channel main
*/


# define BUFFER_LEN			4096
# define SUCCESS			0
# define DISCONNECT			0
# define OPER_PASSWD		"operpass"

int	initServerSocket(unsigned short port);

class Server
{
	private:
		
		typedef								void (Server::*cmdFunction) (std::string&, Client&);
		typedef								std::vector<std::string>::iterator	itVector;

		const std::string					_name;
		int 								_socket;
		std::string							_password;
		std::map<std::string, cmdFunction>	_dico;
		Network								_network;
		std::vector<Client>					_clients;
		std::vector<Channel>				_chans;
		std::vector<Client>					_exceptionlist;

	/* ------------------------------------------------------------------ */

		std::string						findKey(std::string cmd);
		std::vector<std::string>		findArgsCmd(std::string cmd, std::string key);
		bool							checkAvailNick(const std::string str);
		bool							checkValidNick(const std::string& str);
		bool							validChannelName(const std::string& name);
		bool							checkCAP(Client &client, std::string key);
		bool							checkRegistered(cmdFunction f, Client & client);
		void							initDico(void);
		void							callFunCmd(cmdFunction f, Client & client);
		const std::string				prefixServer(void) const;
		void							quitClientCmd(Client &client);
		void							welcomeClient (Client &client);
		Client*							find_client_by_nick(std::string nick);
		Channel*						findChannel(std::string name);
		void							join_channel(Client& client, std::string name, std::string key);

		void							cmd_MODE_user(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_user_add(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_user_remove(std::string& cmd, Client& client,
											std::vector<std::string>& args);									
		
		void							cmd_MODE_channel(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_channel(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_channel_add(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_channel_remove(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_answer(Client & client, std::string& target,
										std::string flag);

		/* CMD */
		void							cmd_CAP(std::string& str, Client& client);
		void							cmd_PASS(std::string& cmd, Client& client);
		void							cmd_NICK(std::string& cmd, Client& client);
		void							cmd_PING(std::string& cmd, Client& client);
		void							cmd_QUIT(std::string& cmd, Client& client);
		void							cmd_USER(std::string& cmd, Client& client);
		void							cmd_JOIN(std::string& cmd, Client& client);
		void							cmd_OPER(std::string& cmd, Client& client);
		void							cmd_KILL(std::string& cmd, Client& client);
		void							cmd_MODE(std::string& cmd, Client& client);
		void							cmd_RESTART(std::string& cmd, Client& client);

		/* ERR */
		void							error_handler(int ERR_CODE, Client &client, const std::string& str = "");

		void							f_ERR_UNKNOWNCOMMAND(Client &client);
		void							f_ERR_WRONGNBPARAMS(Client &client);
		void							f_ERR_ALREADYREGISTERED(Client &client);
		void							f_ERR_PASSWDMISMATCH(Client &client);
		void							f_ERR_NICKNAMEINUSE(Client &client);
		void							f_ERR_ERRONEUSNICKNAME(Client &client);
		void							f_ERR_NONICKNAMEGIVEN(Client &client);
		void							f_ERR_BADCHANMASK(Client &client, const std::string& channel_name);
		void							f_ERR_BADCHANNELKEY(Client &client, const std::string& channel_name);
		void							f_ERR_NOMOTD(Client &client);
		void							f_ERR_NOOPERHOST(Client &client);
		void							f_ERR_NOPRIVILEGES(Client &client);
		void							f_ERR_NOSUCHNICK(Client & client, std::string cmd_str);
		void							f_ERR_NOTREGISTERED(Client& client);
		void							f_ERR_USERSDONTMATCH(Client& client);
		void							f_ERR_UNKNOWNMODE(Client& client, std::string modechar);
		void							f_ERR_UMODEUNKNOWNFLAG(Client & client);
		

		/* RPL */
		void							reply_handler(int RPL_CODE, Client &client, const std::string& str = "");

		void							f_RPL_TOPIC(Client &client, const std::string& channel_name);
		void							f_RPL_NAMREPLY(Client &client, const std::string& channel_name);
		void							f_RPL_ENDOFNAMES(Client &client, const std::string& channel_name);
		void							f_RPL_WELCOME(Client &client);
		void							f_RPL_YOURHOST(Client &client);
		void							f_RPL_CREATED(Client &client);
		void							f_RPL_MYINFO(Client &client);
		void							f_RPL_ISUPPORT(Client &client);
		void							f_RPL_YOUREOPER(Client &client);
		void							f_RPL_UMODEIS(Client &client);
		void							f_RPL_KILLREPLY(Client &client, std::string cible_name,
											Client &killer, std::string &comment);
	public:

		Server(int port, std::string password);
		~Server(void);
		
		
		const int&						getServerSocket(void) const;
		const std::string&				getPassword(void) const;
		const std::vector<Client>&		getClients(void) const;
		const Network&					getNetwork(void) const;


		void							poll(void);
		

		bool							checkSocket(size_t index, short event);

		void							addClient(void);
		void							removeClient(size_t index);
		void							removeClient(Client &client);

		
		int								readQuery(size_t index, char* buffer);
		void							processQuery(int index);

		bool							checkAnswerQuery(size_t index);

	

};
