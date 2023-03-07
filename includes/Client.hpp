#pragma once

# include <string>

int	initClientSocket(int socket_server);

class Client
{
	private:
		std::string	_nickname;
		std::string	_process;
		int			_socket;

	public:
		Client(int socket) :  _socket(initClientSocket(socket))	{}

		const std::string&	getNickname(void) const;
		const int&			getSocket(void) const;

};
