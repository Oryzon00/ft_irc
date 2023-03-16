#pragma once

# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <algorithm>
# include <utility>
# include <errno.h>

# include "RPLERRcode.hpp"
# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"


/* authenticate, set a nickname, a username, join a channel,
send and receive private messages */

/* commands that are specific to operators */

/*
CMD A FAIRE

CONNECTION
	CAP
	PASS
	NICK
	USER
	PING
	PONG
	OPER
	QUIT

Channel Operation
	JOIN message
	PART message
	TOPIC message
	NAMES message
	LIST message
	INVITE message
	KICK message

Sending Messages
	PRIVMSG message
	NOTICE message

User-Based Queries
	WHO message
	WHOIS message

Operator Messages
	KILL message
	RESTART message ??

Server Queries and Commands
	MODE
	MOTD

*/


# define BUFFER_LEN			4096
# define SUCCESS			0
# define DISCONNECT			0




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

	/* ------------------------------------------------------------------ */

		std::string						findKey(std::string cmd);
		std::vector<std::string>		findArgsCmd(std::string cmd, std::string key);
		bool							checkCAP(Client &client, std::string key);
		void							initDico(void);
		void							callFunCmd(cmdFunction f, Client & client);
		const std::string				prefixServer(void) const;
		void							quitClientCmd(Client &client);

		/* CMD */
		void							cmd_CAP(std::string& str, Client& client);
		void							cmd_PASS(std::string& cmd, Client& client);
		
		/* ERR */
		void							error_handler(int ERR_CODE, Client &client);
		void							f_ERR_NEEDMOREPARAMS(Client &client);
		/* RPL */
		void							reply_handler(int RPL_CODE, Client &client);


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
