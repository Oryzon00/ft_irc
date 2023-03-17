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
	OPER	--> Adrian (mettre bool dans client)
	QUIT	

class CHANNELL --> QUENTIN
check registration --> LOUIS
001 - 005 + 251 + 255 + 452 --> LOUIS

Channel Operation
	JOIN message
	PART message
	TOPIC message			Chan Oper Only if trying to change the topic
	NAMES message
	INVITE message			Chan Oper Only
	KICK message			Chan Oper Only

Sending Messages
	PRIVMSG message

Operator Messages
	KILL message	--> Adrian
	RESTART message	--> Adrian

Server Queries and Commands
	MODE		Chan Oper Only if trying to change Chan Modes
	MOTD



*/


# define BUFFER_LEN			4096
# define SUCCESS			0
# define DISCONNECT			0
# define OPER_PASSWD		"operPassword"

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

	/* ------------------------------------------------------------------ */

		std::string						findKey(std::string cmd);
		std::vector<std::string>		findArgsCmd(std::string cmd, std::string key);
		bool							checkAvailNick(const std::string str);
		bool							checkValidName(const std::string& str);
		bool							checkCAP(Client &client, std::string key);
		void							initDico(void);
		void							callFunCmd(cmdFunction f, Client & client);
		const std::string				prefixServer(void) const;
		void							quitClientCmd(Client &client);
		Client*							find_client_by_nick(std::string nick);
		Channel*						findChannel(std::string name);
		void							join_channel(Client& client, std::string name, std::string key);

		/* CMD */
		void							cmd_CAP(std::string& str, Client& client);
		void							cmd_PASS(std::string& cmd, Client& client);
		void							cmd_NICK(std::string& cmd, Client& client);
		void							cmd_PING(std::string& cmd, Client& client);
	//	void							cmd_QUIT(std::string& cmd, Client& client);
		void							cmd_USER(std::string& cmd, Client& client);
		void							cmd_JOIN(std::string& cmd, Client& client);
		void							cmd_OPER(std::string& cmd, Client& client);

		/* ERR */
		void							error_handler(int ERR_CODE, Client &client);

		void							f_ERR_UNKNOWNCOMMAND(Client &client);
		void							f_ERR_WRONGNBPARAMS(Client &client);
		void							f_ERR_ALREADYREGISTERED(Client &client);
		void							f_ERR_PASSWDMISMATCH(Client &client);
		void							f_ERR_NICKNAMEINUSE(Client &client);
		void							f_ERR_ERRONEUSNICKNAME(Client &client);
		void							f_ERR_NONICKNAMEGIVEN(Client &client);
		void							f_ERR_NOOPERHOST(Client &client); //a gerer??
		void							f_ERR_BADCHANNELKEY(Client &client, Channel& channel);


		/* RPL */
		void							reply_handler(int RPL_CODE, Client &client);

		void							f_RPL_YOUREOPER(Client &client);


	public:

		Server(int port, std::string password);
		
		
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
