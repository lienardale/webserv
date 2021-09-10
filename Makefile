# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alienard@student.42.fr <alienard>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/09 12:07:05 by dboyer            #+#    #+#              #
#    Updated: 2021/09/08 13:26:33 by alienard@st      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#									SOURCES
################################################################################

NAME    			=	webserv

MAIN				= 	./srcs/main.cpp

SRCS    			=	socket\
					server \
					config\
					parsing/parser\
					parsing/locationKeys\
					parsing/dataStructure\
					parsing/serverKeys\
					parsing/parsingExceptions\
					parsing/utils\
					utils/ft_itoa\
					utils/strtrim\
					cgi \
					request\
					response\
					statusCode\
					handleGET\
					handleRequest\
					handlePOST\
					handleDELETE\

INCLUDES			=	-I ./includes

HEADERS				=	./includes/webserv.hpp\
					./includes/statusCode.hpp\
					./includes/socket.hpp\
					./includes/server.hpp\
					./includes/response.hpp\
					./includes/request.hpp\
					./includes/config.hpp\
					./includes/cgi.hpp\
					./includes/utils/utils.hpp\
					./includes/parsing/dataStructure.hpp\
					./includes/parsing/parser.hpp\
					./includes/parsing/parsingExceptions.hpp\
					./includes/parsing/utils.hpp\
					./includes/handleRequest.hpp

OBJS_MAIN			= 	$(MAIN:.cpp=.o)
OBJS				= 	$(FIL:.cpp=.o)

FIL = $(addsuffix .cpp, $(addprefix srcs/, $(SRCS)))

CXXFLAGS  			= 	-Werror -Wall -Wextra -std=c++98 -O3 -g ${INCLUDES}
CXX     			= 	clang++
RM      			= 	rm -f

################################################################################
#								Basic Rules
################################################################################

%.o: %.cpp ${HEADERS}
	$(CXX) $(CXXFLAGS) -c $< -o $@

all     :	$(NAME)

$(NAME) :	$(OBJS) $(OBJS_MAIN)
	${CXX} ${CXXFLAGS} -o ${NAME} ${OBJS} ${OBJS_MAIN}

clean   :
	$(RM) $(OBJS)
	$(RM) ${OBJS_MAIN}

fclean  : clean
	$(RM) $(NAME)

re      :	fclean all

################################################################################
#									Extra Rules
################################################################################
run 	:	all
	./${NAME}

valgrind : all
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all ./${NAME}

.PHONY  :	all clean fclean re bonus%
