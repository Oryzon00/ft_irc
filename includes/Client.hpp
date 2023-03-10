#pragma once

# include <string>
# include <vector>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include "CustomException.hpp"

int	initClientSocket(int socket_server);

class Client
{
	private:
		int							_socket;
		std::string					_nickname;
		std::string					_package;
		std::string					_to_send;
		std::vector<std::string>	_cmds;
		
		

	public:
		Client(int socket);
		Client(void);
		~Client(void);

		const int&						getSocket(void) const;
		const std::string&				getNickname(void) const;
		const std::string&				getPackages(void) const;
		const std::string&				getToSend(void) const;
		const std::vector<std::string>&	getCmds(void) const;

		void							setToSend(const std::string& str);
		

		void							readFromClient(char* buffer);
		void							sendToClient(void);
		void							tokenizePack(void);							

};
