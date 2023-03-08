#include "Client.hpp"

const std::string&	Client::getNickname(void) const
{
	return _nickname;
}
const int&			Client::getSocket(void) const
{
	return _socket;
}
