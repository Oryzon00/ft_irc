#include "Channel.hpp"

Channel::Channel(const std::string& name): _name(name) 			{}

Channel::Channel() 												{}

Channel::~Channel() 											{}

Client&						Channel::operator[](size_t index)
{
	return (_members[index]);
}

const std::string&			Channel::getName() const
{
	return (_name);
}

const std::string&			Channel::getTopic() const
{
	return (_topic);
}

void						Channel::addMember(Client& client)
{
	_members.push_back(client);
}

void 						Channel::SendToAll(Client& client)
{
	(void) client;
}