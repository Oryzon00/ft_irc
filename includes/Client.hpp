#pragma once

# include <string>
# include <vector>
# include <cstring>
# include <unistd.h>
# include <sys/socket.h>
# include "CustomException.hpp"
# include <iostream>

# define DELIMITER "\r\n"

int	initClientSocket(int socket_server);

class Client
{
	private:

		int							_socket;
		bool						_isIrssi;
		std::string					_nickname;
	
		std::string					_package; 
		std::string					_cmd; 		//devient une std:string
		std::string					_to_send;	
				
		
		void							clearCmdSend(void); //clear cmd

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
		std::string&					getCmd(void);

		void							printPackage(void);
		void							printCmd(void);
		void							printToSend(void);

		void							setToSend(const std::string& str);
		void							setIsIrssi(const bool& boolean);
		

		void							readBuffer(char* buffer);
		void							sendToClient(void);

		

		// void							tokenizePack(void);	
		bool							checkCmdReady(void);
		void							findCmdInPackage(void);		
	
};


bool	operator==(const Client& lhs, const Client& rhs);
