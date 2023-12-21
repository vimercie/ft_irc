NAME=		ircserv

VPATH=		src/					\
			src/Commands/			\
			src/Commands/Server/	\
			src/Commands/Channel/	\
			src/Commands/Client/

SRC=		main.cpp		\
			Server.cpp		\
			Client.cpp		\
			Channel.cpp		\
			IRCmsg.cpp		\
			Utils.cpp		\
			Exec.cpp		\
			Nick.cpp		\
			User.cpp		\
			Pass.cpp		\
			Privmsg.cpp		\
			Quit.cpp		\
			Join.cpp		\
			Part.cpp		\
			Mode.cpp		\
			Topic.cpp		\
			Kick.cpp		\
			Invite.cpp		\
			Ping.cpp


INCDIR=		inc/

OBJDIR=		obj

INC=		${INCDIR}/*

OBJ=		${addprefix ${OBJDIR}/,		\
			${SRC:.cpp=.o}}

COMP=		c++

FLAGS=		-Wall -Wextra -Werror -std=c++98 -g3

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