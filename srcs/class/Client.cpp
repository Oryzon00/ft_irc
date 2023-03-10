#include "Client.hpp"

Client::Client(int socket)	:  _socket(initClientSocket(socket))	{}

Client::Client(void)		:	_socket(0)							{}

Client::~Client(void)	{ /* close(_socket); */ }

Client&				Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		_socket = rhs._socket;
		_nickname = rhs._nickname;
		_package = rhs._package;
		_to_send = rhs._to_send;
		_cmds = rhs._cmds;
	}
	return (*this);
}

/* --------------------------------------------------------------------------------- */

const int&						Client::getSocket(void) const
{
	return _socket;
}

const std::string&				Client::getNickname(void) const
{
	return _to_send;
}

const std::string&				Client::getPackages(void) const
{
	return _package;
}

const std::string&				Client::getToSend(void) const
{
	return _to_send;
}

const std::vector<std::string>&	Client::getCmds(void) const
{
	return _cmds;
}

void							Client::setToSend(const std::string& str)
{
	_to_send = str;
}

/* --------------------------------------------------------------------------------- */

void				Client::readFromClient(char* buffer)
{
	_package += buffer;
}

void				Client::tokenizePack(void)
{
	std::string	tmp = std::string(_package);
	char* token = strtok(const_cast<char*>(tmp.c_str()), "\n\r");
	if (token)
		_cmds.push_back(std::string(token));
	while (token)
	{
		token = strtok(NULL, "\n\r"); 
		if (token)
			_cmds.push_back(std::string(token));
	}
		
}

void				Client::sendToClient(void)
{
	if (send(_socket, _to_send.c_str(), _to_send.size(), 0) < 0)
		throw SocketException("send()");
	clearPackage();
}

void				Client::clearPackage(void)
{
	_package.clear();
	_to_send.clear();
	_cmds.clear();
}
