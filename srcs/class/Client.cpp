#include "Client.hpp"

Client::Client(int socket)	:  _socket(initClientSocket(socket)), _isIrssi(false)	{}

Client::Client(void)		:	_socket(0), _isIrssi(false)							{}

Client::~Client(void)	{ /* close(_socket); */ }

Client&				Client::operator=(const Client& rhs)
{
	if (this != &rhs)
	{
		_socket = rhs._socket;
		_nickname = rhs._nickname;
		_package = rhs._package;
		_to_send = rhs._to_send;
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

/* --------------------------------------------------------------------------------- */

const int&						Client::getSocket(void) const
{
	return _socket;
}

const bool&						Client::getIsIrssi(void) const
{
	return _isIrssi;
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

std::string&		Client::getCmd(void) 
{
	return _cmd;
}

/* --------------------------------------------------------------------------------- */

/* PRINTER */

void							Client::printPackage(void)
{
	std::cout << _package << std::endl;
}

void							Client::printCmd(void)
{
	std::cout << _cmd << std::endl;
}

void							Client::printToSend(void)
{
	std::cout << _to_send << std::endl;
}



/* --------------------------------------------------------------------------------- */

/* SETTER */

void							Client::setToSend(const std::string& str)
{
	_to_send = str;
}

void							Client::setIsIrssi(const bool& boolean)
{
	_isIrssi = boolean;
}

/* --------------------------------------------------------------------------------- */

void				Client::readBuffer(char* buffer)
{
	if (buffer)
		_package += buffer;
}

/* void				Client::tokenizePack(void)
{
	std::string	tmp = std::string(_package);
	char* token = strtok(const_cast<char*>(tmp.c_str()), "\n\r");
	if (token)
		_cmds.push_back(std::string(token));
	while (token)
	{
		token = strtok(NULL, "\r\n"); 
		if (token)
			_cmds.push_back(std::string(token));
	}
		
} */

void				Client::sendToClient(void) //a re tester
{
	if (_to_send.empty())
	{
		clearCmdSend();
		return ;
	}
	if (send(_socket, _to_send.c_str(), _to_send.size(), 0) < 0)
		throw SocketException("send()");
	clearCmdSend();
}

void				Client::clearCmdSend(void)
{
	_to_send.clear();
	_cmd.clear();
}

// bool				Client::checkCommand (void)
// {
// 	size_t pos = _package.find('\n');
// 	if (pos == std::string::npos) // npos == didn't find anything
// 		return (false);
// 	//if (pos == _package.size())
// 		// save le reste du package dans un temp
// 	return (true);
// }

bool				Client::checkCmdReady(void)
{
	return (!_cmd.empty());
}


void				Client::findCmdInPackage(void) //a tester
{
	size_t	pos = _package.find(DELIMITER);
	if (pos == std::string::npos)
		return ;
	_cmd.insert(0, _package, 0, pos);
	_package.erase(0, pos);
}
