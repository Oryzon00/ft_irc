#pragma once

# include <string>

class Client
{
	private:
		std::string	_nickname;
		int			_socket;

	public:
		Client(std::string nickname, int socket) : _nickname(nickname), _socket(socket)	{}

		const std::string&	getNickname(void) const;
		const int&			getSocket(void) const;

};
