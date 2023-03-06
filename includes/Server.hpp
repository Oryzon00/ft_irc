#pragma once

# include <string>

int	initServerSocket(unsigned short port);

class Server
{
	private:
		int 		_socket;
		std::string	_password;

	public:
		Server(int port, std::string password)
			: _socket(initServerSocket(port)), _password(password)	{}
	

};
