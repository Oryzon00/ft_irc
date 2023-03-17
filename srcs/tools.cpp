#include "tools.hpp"

std::vector<std::string>			strToVec(std::string	str, const std::string& sep)
{
	std::vector<std::string>	vec;
	char	*token = strtok(const_cast<char*>(str.c_str()), const_cast<char*>(sep.c_str()));
	while(token)
	{
		vec.push_back(std::string(token));
		token = strtok(NULL, const_cast<char*>(sep.c_str()));
	}
	return (vec);
}

