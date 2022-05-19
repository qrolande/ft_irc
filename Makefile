NAME	=	irc_serv

SRCS	=	src/main.cpp src/server.cpp src/user.cpp src/utils.cpp

INCL	=	incl/server.hpp incl/user.hpp incl/global.hpp

OBJS	=	$(SRCS:.cpp=.o)

CFLAGS	=	-Wall -Wextra -Werror

CC		=	c++

STD		=	-std=c++98

all:		$(NAME)

%.o: %.cpp	$(INCL)
			@$(CC) $(STD) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
			@$(CC) $(STD) $(OBJS) $(CFLAGS) -o $(NAME)
			@echo "Make done"

clean:
			@${RM} ${OBJS}
			@echo "Make clean done"

fclean:
			@${RM} ${OBJS}
			@${RM} ${NAME}
			@echo "Make fclean done"

re:			fclean all
			@echo "Make re done"

.PHONY:		all clean fclean re
