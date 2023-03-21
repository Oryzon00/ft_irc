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
		bool						_passOK;
		bool						_mode_r;
		bool						_mode_O;
		bool						_mode_i;
		std::string					_nickname;
		std::string					_username;
		std::string					_realname;
		std::string					_package; 
		std::string					_cmd;

	public:
		Client(int socket);
		Client(void);
		~Client(void);

		Client&							operator=(const Client& rhs);							

		const int&						getSocket(void) const;
		const bool&						getIsIrssi(void) const;
		const bool&						getPassOk(void) const;
		const std::string&				getNickname(void) const;
		const std::string&				getUsername(void) const;
		const std::string&				getRealname(void) const;
		const std::string&				getPackages(void) const;
		std::string&					getCmd(void);

		const bool&						getModeR(void) const;
		const bool&						getModeI(void) const;
		const bool&						getModeO(void) const;



		void							setModeR(const bool& boolean);
		void							setModeI(const bool& boolean);
		void							setModeO(const bool& boolean);


		void							printPackage(void);
		void							printCmd(void);

		void							setIsIrssi(const bool& boolean);
		void							setPassOk(const bool& boolean);
		void							setNickname(const std::string& nick);
		void							setUsername(const std::string& str);
		void							setRealname(const std::string& str);
		
		void							clearCmd(void);
		void							readBuffer(char* buffer);
		bool							checkCmdReady(void);
		void							findCmdInPackage(void);
		void							sendToClient(std::string str);	
		//check registration
	
};


bool	operator==(const Client& lhs, const Client& rhs);
bool    operator!=(const Client& lhs, const Client& rhs);

