#include "Client.hpp"

Client::Client(int socket)	:  _socket(initClientSocket(socket))	{}

Client::Client(void)		:	_socket(0)							{}

Client::~Client(void)	{ /* close(_socket); */ }



/* --------------------------------------------------------------------------------- */

const int&			Client::getSocket(void) const
{
	return _socket;
}

const std::string&	Client::getNickname(void) const
{
	return _nickname;
}

const std::string&	Client::getPackages(void) const
{
	return _package;
}

const std::vector<std::string>&	Client::getCmds(void) const
{
	return _cmds;
}


/* --------------------------------------------------------------------------------- */

void				Client::readPackage(char* buffer)
{
	_package += buffer;
}

void				Client::tokenizePack(void)
{
	char* token = strtok(const_cast<char*>(_package.c_str()), "\n\r");
	if (token)
		_cmds.push_back(std::string(token));
	while (token)
	{
		token = strtok(NULL, "\n\r"); 
		if (token)
			_cmds.push_back(std::string(token));
	}
		
}
