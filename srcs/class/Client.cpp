#include "Client.hpp"

Client::Client(const Client& client)
{
	*this = client;
}

Client::Client(int socket)	:
		_socket(initClientSocket(socket)), _isIrssi(false), _passOK(false), _mode_r(false),
		_mode_O(false), _mode_i(false), _nickname("*")	{}


Client::Client(void)		:
		_socket(0), _isIrssi(false), _passOK(false), _mode_r(false),
		_mode_O(false), _mode_i(false), _nickname("*")	{}


Client::~Client(void)	{ /* close(_socket); */ }

Client&				Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		_socket = rhs._socket;
		_isIrssi = rhs._isIrssi;
		_passOK = rhs._passOK;
		_mode_r = rhs._mode_r;
		_mode_O = rhs._mode_O;
		_mode_i = rhs._mode_i;
		_nickname = rhs._nickname;
		_username = rhs._username;
		_realname = rhs._realname;
		_package = rhs._package;
		_cmd = rhs._cmd;
	}
	return (*this);
}

bool	operator==(const Client& lhs, const Client& rhs)
{
	if (lhs.getSocket() == rhs.getSocket())
		return true;
	else
		return false;
}

bool	operator!=(const Client& lhs, const Client& rhs)
{
	return(!(lhs == rhs));
}

/* --------------------------------------------------------------------------------- */

/* GETTER */

const int&						Client::getSocket(void) const
{
	return _socket;
}

const bool&						Client::getIsIrssi(void) const
{
	return _isIrssi;
}

const bool&						Client::getPassOk(void) const
{
	return _passOK;
}

const bool&						Client::getModeR(void) const
{
	return _mode_r;
}

const bool&						Client::getModeO(void) const
{
	return _mode_O;
}

const bool&						Client::getModeI(void) const
{
	return _mode_i;
}

const std::string&				Client::getNickname(void) const
{
	return _nickname;
}

const std::string&				Client::getUsername(void) const
{
	return	_username;
}

const std::string&				Client::getRealname(void) const
{
	return _realname;
}

const std::string&				Client::getPackages(void) const
{
	return _package;
}

std::string&		Client::getCmd(void) 
{
	return _cmd;
}

/* --------------------------------------------------------------------------------- */

/* PRINTER */

void							Client::printPackage(void)
{
	std::cout  << "----- RESTE PACKAGE -----" << std::endl;
	std::cout << _package << std::endl;
}

void							Client::printCmd(void)
{
	std::cout  << "----- CMD -----" << std::endl;
	std::cout << _cmd << std::endl;
}



/* --------------------------------------------------------------------------------- */

/* SETTER */

void							Client::setIsIrssi(const bool& boolean)
{
	_isIrssi = boolean;
}

void							Client::setModeR(const bool& boolean)
{
	_mode_r = boolean;
}

void							Client::setPassOk(const bool& boolean)
{
	_passOK = boolean;
}

void							Client::setModeO(const bool& boolean)
{
	_mode_O = boolean;
}

void							Client::setModeI(const bool& boolean)
{
	_mode_i = boolean;
}

void							Client::setNickname(const std::string& nick)
{
	_nickname = nick;
}

void							Client::setUsername(const std::string& str)
{
	_username = str;
}

void							Client::setRealname(const std::string& str)
{
	_realname = str;
}

/* --------------------------------------------------------------------------------- */

/* PRIVATE FUNCTION */

void				Client::clearCmd(void)
{
	_cmd.clear();
}

/* --------------------------------------------------------------------------------- */

/* PUBLIC FUNCTION */

void				Client::readBuffer(char* buffer)
{
	if (buffer)
		_package += buffer;
}

void				Client::sendToClient(std::string str)
{
	// if (send(_socket, str.c_str(), str.size(), 0) < 0) --> protection interdite par le sujet
	// 	throw SocketException("send()");
	send(_socket, str.c_str(), str.size(), MSG_NOSIGNAL); //0 --> MSG_NO_SIGNAL
}



bool				Client::checkCmdReady(void)
{
	return (!_cmd.empty());
}


void				Client::findCmdInPackage(void)
{
	size_t	pos = _package.find(DELIMITER);
	if (pos == std::string::npos)
		return ;
	_cmd.insert(0, _package, 0, pos);
	_package.erase(0, pos + strlen(DELIMITER));
}
