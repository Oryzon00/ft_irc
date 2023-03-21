#include "Channel.hpp"

Channel::Channel(Client& client, const std::string& name, const std::string& key): _name(name), _topic("No Topic"), _key(key)
{
	_members.push_back(client);
}

Channel::Channel() 												{}

Channel::~Channel() 											{}

Channel&				Channel::operator=(const Channel& rhs)
{
	if (this != &rhs)
	{
		_name = rhs._name;
		_topic = rhs._topic;
		_key = rhs._key;
		_members = rhs._members;
	}
	return (*this);
}

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

int							Channel::size() const
{
	return (_members.size());
}

bool						Channel::isChanOp(Client& client)
{
	return (_members[0].getNickname() == client.getNickname());
}

void						Channel::addMember(Client& client)
{
	_members.push_back(client);
//	SendToAll(client.getNickname() + " JOIN :" + getName() + "\n");
}

void						Channel::removeMember(Client& client)
{
	//SendToAll(client.getNickname() + " PART " + getName() + "\n");
	for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (it->getNickname() == client.getNickname())
		{
			_members.erase(it);
			return ;
		}
	}
}

void 						Channel::SendToAll(const std::string& str)
{
	for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
		it->sendToClient(str);
}