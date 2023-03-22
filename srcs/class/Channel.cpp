#include "Channel.hpp"

Channel::Channel(Client& founder, const std::string& name, const std::string& key,
				std::vector<Client>* exceptionlist) :
	_name(name), _topic("No Topic"), _key(key), _exceptionlist(exceptionlist),
	_mode_i(false), _mode_m(false), _mode_s(false), _mode_t(true)
{
	_members.push_back(founder);
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
		_banlist = rhs._banlist;
		_invitelist = rhs._invitelist;
		_exceptionlist = rhs._exceptionlist;
		_mode_i = rhs._mode_i;
		_mode_m = rhs._mode_m;
		_mode_s = rhs._mode_s;
		_mode_t = rhs._mode_t;

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

const bool&			Channel::getModeT() const
{
	return (_mode_t);
}

const bool&			Channel::getModeM() const
{
	return(_mode_m);
}

void	Channel::setTopic(const std::string &str)
{
	_topic = str;
}

int							Channel::size() const
{
	return (_members.size());
}

bool						Channel::isChanOp(Client& client)
{
	return (_members[0] == client);
}

bool						Channel::isBanned(Client& client)
{
	for(std::vector<Client>::iterator it = _banlist.begin(); it != _banlist.end(); it++)
	{
		if (client == *it)
			return (true);
	}
	return (false);
}

bool						Channel::isInvited(Client& client)
{
	if (_mode_i == false)
		return (true);
	for (std::vector<Client>::iterator it = _exceptionlist->begin(); it != _exceptionlist->end(); it++)
	{
		if (client == *it)
			return (true);
	}
	for (std::vector<Client>::iterator it = _invitelist.begin(); it != _invitelist.end(); it++)
	{
		if (client == *it)
			return (true);
	}
	return (false);
}

bool                        Channel::isMember(Client &client)
{
    for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if (*it == client)
            return (true);
    }
    return (false);
}

void						Channel::addMember(Client client)
{
	_members.push_back(client);
}

void						Channel::removeMember(Client& client)
{
	for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (it->getNickname() == client.getNickname())
		{
			_members.erase(it);
			return ;
		}
	}
}

void 						Channel::SendToAll(Client& client, const std::string& str)
{
	for(std::vector<Client>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (*it != client)
			it->sendToClient(str);
	}
}

