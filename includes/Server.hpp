#pragma once

# include <string>
# include "Client.hpp"
# include <vector>

int	initServerSocket(unsigned short port);

class Server
{
	private:
		int 				_socket;
		std::string			_password;
		std::vector<Client>	_clients;

	public:

		Server(int port, std::string password)
			: _socket(initServerSocket(port)), _password(password)	{}
		
		void	addClient(Client client);
		
		const int&						getServerSocket(void) const;
		const std::string&				getPassword(void) const;
		const std::vector<Client>&		getClients(void) const;
	

};
