#pragma once 

# include <vector>
# include "Client.hpp"

class Channel
{
	private:
		const std::string		_name;
		std::string				_topic;
		std::string				_key; // ????
		std::vector<Client>		_members;

	public:
		Channel();
		Channel(const std::string& name);
		~Channel(void);

		Client&			operator[](size_t index);

		const std::string&		getName() const;
		const std::string&		getTopic() const;

		void			addMember(Client& client);
		void			SendToAll(Client& client);
};