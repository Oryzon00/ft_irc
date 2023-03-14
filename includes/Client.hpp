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
		bool						_isIrssi;
		std::string					_nickname;
		std::string					_package;
		std::string					_to_send;
		std::vector<std::string>	_cmds; //devient une std:string		
		
		void							clearPackage(void); //clear cmd

	public:
		Client(int socket);
		Client(void);
		~Client(void);

		Client&							operator=(const Client& rhs);							

		const int&						getSocket(void) const;
		const bool&						getIsIrssi(void) const;
		const std::string&				getNickname(void) const;
		const std::string&				getPackages(void) const;
		const std::string&				getToSend(void) const;
		std::vector<std::string>&		getCmds(void);

		void							setToSend(const std::string& str);
		void							setIsIrssi(const bool& boolean);
		

		void							readFromClient(char* buffer);
		void							sendToClient(void);
		void							tokenizePack(void);			
	
									
		// bool							checkCommand(void); // Attention si il reste qq chose apres '\n' !

};


bool	operator==(const Client& lhs, const Client& rhs);
