#include "Channel.hpp"

Channel::Channel(Client* founder, const std::string& name, const std::string& key) :
	_name(name), _topic("No Topic"), _key(key),
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
		// _banlist = rhs._banlist;
		_invitelist = rhs._invitelist;
		_mode_i = rhs._mode_i;
		_mode_m = rhs._mode_m;
		_mode_s = rhs._mode_s;
		_mode_t = rhs._mode_t;

	}
	return (*this);
}

Client*					Channel::operator[](size_t index)
{
	return (_members[index]);
}

bool						operator==(const Channel& lhs,const Channel& rhs)
{
	return (lhs.getName() == rhs.getName());
}


const std::string&			Channel::getName() const
{
	return (_name);
}

const std::string&		Channel::getTopic() const
{
	return (_topic);
}

const std::string&		Channel::getKey() const
{
	return (_key);
}

const bool&				Channel::getModeT() const
{
	return (_mode_t);
}

const bool&				Channel::getModeM() const
{
	return(_mode_m);
}

const bool&				Channel::getModeI(void) const
{
	return (_mode_i);
}

const bool&				Channel::getModeS(void) const
{
	return (_mode_s);
}

/* --------------------------------------------------------------------------------- */


void					Channel::setTopic(const std::string &str)
{
	_topic = str;
}

void					Channel::setModeI(const bool& boolean)
{
	_mode_i = boolean;
}

void					Channel::setModeM(const bool& boolean)
{
	_mode_m = boolean;
}

void					Channel::setModeS(const bool& boolean)
{
	_mode_s = boolean;
}

void					Channel::setModeT(const bool& boolean)
{
	_mode_t = boolean;
}

/* --------------------------------------------------------------------------------- */


int						Channel::size() const
{
	return (_members.size());
}

bool					Channel::isChanOp(Client& client)
{
	return (_members[0] == &client);
}

// bool					Channel::isBanned(Client& client)
// {
// 	for(std::vector<Client*>::iterator it = _banlist.begin(); it != _banlist.end(); it++)
// 	{
// 		if (&client == *it)
// 			return (true);
// 	}
// 	return (false);
// }

bool					Channel::isInvited(Client& client)
{
	if (_mode_i == false)
		return (true);
	for (std::vector<Client*>::iterator it = _invitelist.begin(); it != _invitelist.end(); it++)
	{
		if (&client == *it)
			return (true);
	}
	return (false);
}

bool					Channel::isMember(Client &client)
{
    for(std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
    {
        if (*it == &client)
            return (true);
    }
    return (false);
}

void					Channel::invite(Client* client)
{
	_invitelist.push_back(client);
}


void					Channel::addMember(Client* client)
{
	_members.push_back(client);
}

void						Channel::removeMember(Client& client)
{
	for(std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (*it == &client)
		{
			_members.erase(it);
			return ;
		}
	}
}

void 					Channel::SendToAll(Client& client, const std::string& str)
{
	for(std::vector<Client*>::iterator it = _members.begin(); it != _members.end(); it++)
	{
		if (*it != &client)
		{
			Client tmp = *(*it);
			tmp.sendToClient(str);
		}
	}
}
