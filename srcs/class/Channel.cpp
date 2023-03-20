#include "Channel.hpp"

Channel::Channel(Client& client, const std::string& name, const std::string& key): _name(name), _key(key)
{
	_members.push_back(client);
}

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

const std::string&			Channel::getKey() const
{
	return (_key);
}

void						Channel::addMember(Client& client)
{
	SendToAll(client.getNickname() + " JOIN " + getName() + "\n");
	_members.push_back(client);
}

void 						Channel::SendToAll(const std::string& str)
{
	for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
		it->sendToClient(str);
}