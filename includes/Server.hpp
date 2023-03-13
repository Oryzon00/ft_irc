#pragma once

# include <string>
# include <vector>
# include <map>
# include <iostream>

# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"


# define BUFFER_LEN			1024

int	initServerSocket(unsigned short port);


class Server
{
	private:

		typedef								void(*cmdFunction)(std::string&, Client&);
		typedef								std::vector<std::string>::iterator	itVector;

		int 								_socket;
		std::string							_password;
		std::map<std::string, cmdFunction>	_dico;
		Network								_network;
		std::vector<Client>					_clients;



	public:

		Server(int port, std::string password);
		
		
		
		const int&						getServerSocket(void) const;
		const std::string&				getPassword(void) const;
		const std::vector<Client>&		getClients(void) const;
		const Network&					getNetwork(void) const;


		void							poll(void);
		
		void							sendPackages(Client & client);

		bool							checkSocket(size_t index, short event);

		void							addClient(void);
		void							removeClient(size_t index);

		std::string						getKey(std::string cmd);
		int								readQuery(size_t index, char* buffer);
		void							processQuery(int index);


	

};
