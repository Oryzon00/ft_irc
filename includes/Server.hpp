#pragma once

# include <string>
# include <vector>

# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"

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

		/*
		Fonction membre a coder:
			- pollserver ***
			- addClient	 ***
			- removeClient
			- printMsg
			- bool checkSocket(size_t i) ***
		*/

		void							poll(void);
		size_t							_recv(size_t index, char* buffer);

		bool							checkSocket(size_t index, short event);

		void							addClient(void);
		void							removeClient(Client& client);


	

};
