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
		Channel(Client& client, const std::string& name, const std::string& key);
		~Channel(void);

		Client&			operator[](size_t index);

		const std::string&		getName() const;
		const std::string&		getTopic() const;
		const std::string&		getKey() const;

		void			addMember(Client& client);
		void			SendToAll(const std::string& str);
};