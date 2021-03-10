# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dboyer <dboyer@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/03/09 12:07:05 by dboyer            #+#    #+#              #
#    Updated: 2021/03/09 18:26:20 by dboyer           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

################################################################################
#									SOURCES
################################################################################

NAME    			=	webserv

MAIN				= 	./srcs/main.cpp

SRCS    			= 	./srcs/test.cpp

INCLUDES			=	-I ./includes

HEADERS				=	./includes/webserv.hpp

OBJS_MAIN			= 	$(MAIN:.cpp=.o)
OBJS				= 	$(SRCS:.cpp=.o)

CXXFLAGS  			= 	-Werror -Wall -Wextra -std=c++98 -O3 ${INCLUDES}
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

.PHONY  :	all clean fclean re bonus%