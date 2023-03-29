#pragma once 

# include <vector>
# include "Client.hpp"

class Channel
{
	private:
		std::string					_name;
		std::string					_topic;
		std::string					_key;
		std::vector<unsigned long>	_members;
		std::vector<unsigned long>	_invitelist;	

		bool						_mode_i;
		bool						_mode_m;
		bool						_mode_s;
		bool						_mode_t;
		
	public:
		Channel();
		Channel(Client* client, const std::string& name, const std::string& key); //garder a modif
		~Channel(void);

		unsigned long			operator[](size_t index);// garder  a modif
		Channel&				operator=(const Channel& rhs);

		const std::string&		getName() const;
		std::vector<unsigned long>&			getMembers();

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

		bool					isChanOp(Client& client); // garder  a modif
		bool					isInvited(Client& client); // garder a modif
		bool					isMember(Client &client); // garder a modif
		
		void					invite(Client* client); // garder a modif
		void					addMember(Client* client); // garder a modif
		void					removeMember(Client& client); // garder a modif
		void					removeMember(Client* client); // garder a modif
		// void					SendToAll(Client& client, const std::string& str);
};

bool	operator==(const Channel& lhs,const Channel& rhs);