#pragma once 

# include <vector>
# include "Client.hpp"

class Channel
{
	private:
		std::string				_name;
		std::string				_topic;
		std::string				_key;			//mode k
		std::vector<Client>		_members;
		std::vector<Client>		_banlist;		//mode b
		std::vector<Client>		_invitelist;	//mode i
		std::vector<Client>*	_exceptionlist;	//mode e

		bool					_mode_i;		//invite --> false
		bool					_mode_m;		//moderated	--> false
		bool					_mode_s;		//secret --> false
		bool					_mode_t;		//topic --> true





	public:
		Channel();
		Channel(Client& client, const std::string& name, const std::string& key,
				std::vector<Client>* exceptionlist);
		~Channel(void);

		Client&					operator[](size_t index);
		Channel&				operator=(const Channel& rhs);

		const std::string&		getName() const;
		const std::string&		getTopic() const;
		const std::string&		getKey() const;

		const bool&				getModeI(void) const;
		const bool&				getModeM(void) const;
		const bool&				getModeS(void) const;
		const bool&				getModeT(void) const;

		void					setModeI(const bool& boolean);
		void					setModeM(const bool& boolean);
		void					setModeS(const bool& boolean);
		void					setModeT(const bool& boolean);

		int						size() const;

		bool					isChanOp(Client& client);
		bool					isBanned(Client& client);
		bool					isInvited(Client& client);
		bool					isMember(Client &client);

		
		void					addMember(Client client);
		void					removeMember(Client& client);
		void					SendToAll(const std::string& str);
};
