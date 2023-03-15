#pragma once

# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <algorithm>
# include <utility>
# include <errno.h>

# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"


# define BUFFER_LEN			4096
# define SUCCESS			0
# define DISCONNECT			0

int	initServerSocket(unsigned short port);


class Server
{
	private:
		const std::string					_name =  "13-20h_IRC";

		typedef								bool (Server::*cmdFunction) (std::string&, Client&);
		typedef								std::vector<std::string>::iterator	itVector;

		int 								_socket;
		std::string							_password;
		std::map<std::string, cmdFunction>	_dico;
		Network								_network;
		std::vector<Client>					_clients;



		std::string						findKey(std::string cmd);
		std::vector<std::string>		findArgsCmd(std::string cmd, std::string key);
		bool							checkCAP(Client &client, std::string key);
		void							initDico(void);
		void							callFunCmd(cmdFunction f, Client & client);

		
		// void							sendPackages(Client & client);


		/* CMD */
		bool							cmd_CAP(std::string& str, Client& client);
		bool							cmd_PASS(std::string& cmd, Client& client);
		



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
