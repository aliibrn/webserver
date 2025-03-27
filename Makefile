NAME = webserv

SRCS = webserver.cpp \
	set_config.cpp \
	HttpRequest.cpp \

OBJS = ${SRCS:.cpp=.o}
CXX = c++ 
CXXFLAGS =  -std=c++98 #-Wall -Wextra -Werror
RM = rm -f

.c.o:
	@$(CXX) $(CXXFLAGS) -c $< -o $@

${NAME}: ${OBJS}
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) 

all: ${NAME}

clean:
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME} 

re: fclean all

.PHONY : all clean fclean rec
