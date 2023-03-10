#pragma once

# include <string>
# include <vector>
# include <cstring>
# include <unistd.h>

int	initClientSocket(int socket_server);

class Client
{
	private:
		int							_socket;
		std::string					_nickname;
		std::string					_package;
		std::vector<std::string>	_cmds;
		

	public:
		Client(int socket);
		Client(void);
		~Client(void);

		const int&						getSocket(void) const;
		const std::string&				getNickname(void) const;
		const std::string&				getPackages(void) const;
		const std::vector<std::string>&	getCmds(void) const;

		void							readPackage(char* buffer);
		void							tokenizePack(void);

};
