#pragma once

# include <string>
# include <vector>
# include <iostream>

# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"

# define BUFFER_LEN			1024

int	initServerSocket(unsigned short port);


class Server
{
	private:
		int 				_socket;
		std::string			_password;
		std::vector<Client>	_clients;
		Network				_network;

	public:

		Server(int port, std::string password);
		
		
		
		const int&						getServerSocket(void) const;
		const std::string&				getPassword(void) const;
		const std::vector<Client>&		getClients(void) const;
		const Network&					getNetwork(void) const;


		void							poll(void);
		int								readPackages(size_t index, char* buffer);
		void							sendPackages(size_t index);

		bool							checkSocket(size_t index, short event);

		void							addClient(void);
		void							removeClient(size_t index);

		void							processQuery(int index);


	

};
