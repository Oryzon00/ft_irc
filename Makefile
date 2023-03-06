NAME		=	ircserv

SRCS		=	srcs/main.cpp				\
				srcs/parsing.cpp			\
				srcs/initServerSocket.cpp	\

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

.PHONY		:	clean fclean re all
