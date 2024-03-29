#pragma once

# include <string>
# include <vector>
# include <map>
# include <list>
# include <iostream>
# include <algorithm>
# include <utility>
# include <sstream>
# include <errno.h>

# include "RPLERRcode.hpp"
# include "Client.hpp"
# include "Network.hpp"
# include "CustomException.hpp"
# include "Channel.hpp"
# include "tools.hpp"

# define BUFFER_LEN			4096
# define SUCCESS			0
# define DISCONNECT			0
# define OPER_NAME			"oper"
# define OPER_PASSWD		"operpass"

# define SIGN_NONE			0
# define SIGN_PLUS			1
# define SIGN_MINUS			2

int	initServerSocket(unsigned short port);

class Server
{
	private:
		
		typedef								void (Server::*cmdFunction) (std::string&, Client&);
		typedef								std::vector<std::string>::iterator	itVector;

		const std::string					_name;
		int 								_socket;
		unsigned long						_id_count;
		std::string							_password;
		std::map<std::string, cmdFunction>	_dico;
		Network								_network;
		std::vector<Client>					_clients;
		std::vector<Channel>				_chans;
		std::string 						_MOTD;
		Client								_bot;
		std::string							_opername;
		std::string							_operpass;
	
	/* ------------------------------------------------------------------ */

		std::string						findKey(std::string cmd);
		std::vector<std::string>		findArgsCmd(std::string cmd, std::string key);
		bool							checkAvailNick(const std::string str);
		bool							checkValidNick(const std::string& str);
		bool							validChannelName(const std::string& name);
		bool							checkCAP(Client &client, std::string key);
		bool							checkRegistered(cmdFunction f, Client & client);
		bool							checkOP(Client& client, Channel& channel);
		void							initDico(void);
		void							initBot(void);
		void							callFunCmd(cmdFunction f, Client & client);
		const std::string				prefixServer(void) const;
		void							quitClientCmd(Client &client);
		void							welcomeClient (Client &client);
		Client*							find_client_by_nick(std::string nick);

		Client*							find_client_by_id(unsigned long id);
		void							sendToChannel(Channel* channel, Client& sender, const std::string& str);
		
		Channel*						findChannel(std::string name);
		void							join_channel(Client& client, std::string name, std::string key);
		void							message_to_channel(std::string channelTargetName, Client& client, std::string message);
		void							message_to_client(std::string clientTargetName, Client& client, std::string message);
		void							notice_to_channel(std::string channelTargetName, Client& client, std::string message);
		void							notice_to_client(std::string clientTargetName, Client& client, std::string message);
		void							part_channel(Client& client, std::string name, std::string reason);
		void							quit_channel(Client& client, std::string name, std::string reason);
		void							leaveAllChannels(Client& client, std::string reason);

		void							BotProcess(Client& client, const std::string& message);
		void							BotCmdHelp(Channel* channel);
		void							BotCmdPing(Channel* channel);
		void							BotCmdUnknown(Channel* channel);
		void							BotCmdBunny(Channel* channel);
		void							BotCmdUsers(Channel* channel);

		void							cmd_MODE_user(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_user_add(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_user_remove(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_channel(std::string& cmd, Client& client,
											std::vector<std::string>& args);;
		void							cmd_MODE_channel_parse(std::string& cmd, Client& client,
											std::vector<std::string>& args);
		void							cmd_MODE_channel_i(Client& client, Channel* channel,
											std::string& channel_name, char& sign, char mode);
		void							cmd_MODE_channel_m(Client& client, Channel* channel,
											std::string& channel_name, char& sign, char mode);
		void							cmd_MODE_channel_s(Client& client, Channel* channel,
											std::string& channel_name, char& sign, char mode);
		void							cmd_MODE_channel_t(Client& client, Channel* channel,
											std::string& channel_name, char& sign, char mode);
		void							cmd_MODE_answer(Client & client, std::string& target,
											std::string flag);
		void							cmd_MODE_answer_channel(Client & client,
											std::string& target, std::string flag, Channel* channel);

		/* CMD */
		void							cmd_CAP(std::string& str, Client& client);
		void							cmd_PASS(std::string& cmd, Client& client);
		void							cmd_NICK(std::string& cmd, Client& client);
		void							cmd_PING(std::string& cmd, Client& client);
		void							cmd_QUIT(std::string& cmd, Client& client);
		void							cmd_USER(std::string& cmd, Client& client);
		void							cmd_JOIN(std::string& cmd, Client& client);
		void							cmd_OPER(std::string& cmd, Client& client);
		void							cmd_KILL(std::string& cmd, Client& client);
		void							cmd_PRIVMSG(std::string& cmd, Client& client);
		void							cmd_MODE(std::string& cmd, Client& client);
		void							cmd_RESTART(std::string& cmd, Client& client);
		void							cmd_TOPIC(std::string& cmd, Client& client);
		void							cmd_PART(std::string& cmd, Client& client);
		void							cmd_MOTD(std::string& cmd, Client& client);
		void							cmd_NAMES(std::string& cmd, Client& client);
		void							cmd_LIST(std::string& cmd, Client& client);
		void							cmd_WHO(std::string& cmd, Client& client);
		void							cmd_WHOIS(std::string& cmd, Client& client);
		void							cmd_INVITE(std::string& cmd, Client& client);
		void							cmd_KICK(std::string& cmd, Client& client);
		void							cmd_NOTICE(std::string& cmd, Client& client);

		/* ERR */
		void							error_handler(int ERR_CODE, Client &client, const std::string& str = "");

		void							f_ERR_UNKNOWNCOMMAND(Client &client);
		void							f_ERR_WRONGNBPARAMS(Client &client);
		void							f_ERR_ALREADYREGISTERED(Client &client);
		void							f_ERR_PASSWDMISMATCH(Client &client);
		void							f_ERR_NICKNAMEINUSE(Client &client);
		void							f_ERR_ERRONEUSNICKNAME(Client &client);
		void							f_ERR_NONICKNAMEGIVEN(Client &client);
		void							f_ERR_BADCHANMASK(Client& client, const std::string& channel_name);
		void							f_ERR_BADCHANNELKEY(Client& client, const std::string& channel_name);
		void							f_ERR_BANNEDFROMCHAN(Client& client, const std::string& channel_name);
		void							f_ERR_INVITEONLYCHAN(Client& client, const std::string& channel_name);
		void							f_ERR_NOMOTD(Client& client);
		void							f_ERR_NOOPERHOST(Client& client);
		void							f_ERR_NOPRIVILEGES(Client& client);
		void							f_ERR_NOSUCHNICK(Client& client, std::string cmd_str);
		void							f_ERR_NOTREGISTERED(Client& client);
		void							f_ERR_USERSDONTMATCH(Client& client);
		void							f_ERR_UNKNOWNMODE(Client& client, std::string modechar);
		void							f_ERR_UMODEUNKNOWNFLAG(Client & client);
		void							f_ERR_NORIGHT(Client & client);
		void							f_ERR_CANNOTSENDTOCHAN(Client& client, const std::string& channel_name);
		void							f_ERR_CHANOPRIVSNEEDED(Client& client, const std::string& channel_name);
		void							f_ERR_NOTONCHANNEL(Client &client, const std::string& channel_name);
		void							f_ERR_NOSUCHCHANNEL(Client &client, const std::string& channel_name);
		void							f_ERR_USERONCHANNEL(Client &client, const std::string& channel_name);
		void							f_ERR_USERNOTINCHANNEL(Client &client, const std::string& channel_name);
		void							f_ERR_NOLS(Client& client);
		void							f_ERR_NOCAP(Client& client);
		void							f_ERR_NOPASS(Client& client);



	/* RPL */
		void							reply_handler(int RPL_CODE, Client &client, const std::string& str = "");

		void							f_RPL_TOPIC(Client& client, const std::string& channel_name);
		void							f_RPL_NAMREPLY(Client& client, const std::string& channel_name);
		void							f_RPL_ENDOFNAMES(Client &client, const std::string& channel_name);
		void							f_RPL_WELCOME(Client& client);
		void							f_RPL_YOURHOST(Client& client);
		void							f_RPL_CREATED(Client& client);
		void							f_RPL_MYINFO(Client& client);
		void							f_RPL_ISUPPORT(Client& client);
		void							f_RPL_YOUREOPER(Client& client);
		void							f_RPL_UMODEIS(Client& client);
		void							f_RPL_CHANNELMODEIS(Client &client, Channel& channel);
		void							f_RPL_KILLREPLY(Client &client, std::string cible_name,
											Client &killer, std::string &comment);
		void							f_RPL_NOTOPIC(Client& client, std::string channel_name);
		void							f_RPL_MOTDSTART(Client& client);
		void							f_RPL_MOTD(Client& client);
		void							f_RPL_ENDOFMOTD(Client& client);
		void							f_RPL_LISTSTART(Client& client);
		void							f_RPL_LIST(Client& client, std::string channel_name);
		void							f_RPL_LISTEND(Client& client);
		void							f_RPL_INVITING(Client &client, std::string s);



	public:

		Server(int port, std::string password);
		~Server(void);
		
		
		const int&						getServerSocket(void) const;
		const std::string&				getPassword(void) const;
		const std::vector<Client>&		getClients(void) const;
		const Network&					getNetwork(void) const;


		void							poll(void);
		

		bool							checkSocket(size_t index, short event);

		void							addClient(void);
		void							removeClient(size_t index);
		void							removeClient(Client &client);
		void							removeChannel(Channel channel);

		
		int								readQuery(size_t index, char* buffer);
		void							processQuery(int index);

		bool							checkAnswerQuery(size_t index);

	

};
