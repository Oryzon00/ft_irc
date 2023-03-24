#pragma once 

# include <vector>
# include "Client.hpp"

class Channel
{
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_key;
		std::vector<Client*>		_members;
		// std::vector<Client*>		_banlist;
		std::vector<Client*>		_invitelist;	//mode i

		bool						_mode_i;
		bool						_mode_m;
		bool						_mode_s;
		bool						_mode_t;
		
	public:
		Channel();
		Channel(Client* client, const std::string& name, const std::string& key);
		~Channel(void);

		Client*					operator[](size_t index);
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
		void					setTopic(const std::string& str);

		int						size() const;

		bool					isChanOp(Client& client);
		// bool					isBanned(Client& client);
		bool					isInvited(Client& client);
		bool					isMember(Client &client);
		
		void					invite(Client* client);
		void					addMember(Client* client);
		void					removeMember(Client& client);
		void					removeMember(Client* client);
		void					SendToAll(Client& client, const std::string& str);
};

bool	operator==(const Channel& lhs,const Channel& rhs);