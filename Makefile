NAME=		ircserv

VPATH=		src/

SRC=		main.cpp		\
			Server.cpp		\
			Channel.cpp		\
			IRCMessage.cpp

INCDIR=		inc/

OBJDIR=		obj

INC=		${INCDIR}/*

OBJ=		${addprefix ${OBJDIR}/,		\
			${SRC:.cpp=.o}}

COMP=		c++

FLAGS=		-Wall -Wextra -Werror -std=c++98

all:		${OBJDIR} ${NAME}

${NAME}:	${OBJ}
			${COMP} ${OBJ} -o ${NAME}

${OBJDIR}:
			mkdir -p obj

${OBJDIR}/%.o:	%.cpp ${INC} Makefile
			${COMP} ${FLAGS} -c $< -o $@

clean:
			rm -rf ${OBJDIR}

fclean:		clean
			${RM} ${NAME}

re:			fclean all

.PHONY:		all clean fclean re