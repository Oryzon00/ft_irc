NAME		=	ircserv

SRCS		=	srcs/main.cpp				\
				srcs/parsing.cpp			\
				srcs/initSocket.cpp			\
				srcs/class/Client.cpp		\
				srcs/class/Server.cpp		\
				srcs/class/Servercmd.cpp	\
				srcs/class/Network.cpp		\
				srcs/class/Channel.cpp		\
				srcs/execLoop.cpp			\
				srcs/signal.cpp				\
				srcs/tools.cpp				\


INC_DIR		=	includes

CXX			=	c++

CXXFLAGS	=	-Wall -Werror -Wextra -std=c++98

DEBUG		=	-g3

RM			=	rm -f

OBJS		=	$(SRCS:.cpp=.o)

all			:	$(NAME) 

%.o : %.cpp
			$(CXX) $(CXXFLAGS) $(DEBUG) -I $(INC_DIR) -o $@ -c $< 

$(NAME)		:	$(OBJS)
				$(CXX)  $(OBJS) -o $(NAME) 
		
clean		:	
				$(RM) $(OBJS)

fclean		:	clean
				$(RM) $(NAME)

re			:	fclean all

test		:	$(NAME)
				valgrind --track-fds=yes ./$(NAME) 1234 password

.PHONY		:	clean fclean re all test
