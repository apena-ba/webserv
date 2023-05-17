#\\\\\\\\\\\\\\\\\\\\\\\  NTAMAYO-'s Amazing Makefile  ////////////////////////#

NAME		= webserv

CXX			= c++

CXXFLAGS	= -Wall -Wextra -Werror -std=c++98

INC			= -I include -I include/ConfigurationParser

VPATH		= ./src:./src/ConfigurationParser/:./src/ConfigurationParser/utils/

#----                                                                      ----#

CORESRCS	= $(addprefix src/, Cluster.cpp Server.cpp Port.cpp Cgi.cpp)
CONFSRCS	= $(addprefix src/ConfigurationParser/, ConfigurationParser.cpp utils/_TempConfiguration.cpp utils/_TempRoute.cpp utils/ParsingUtils.cpp)
HTTPSRCS	= $(addprefix src/, HTTPRequestParser.cpp HTTPResponse.cpp)
#------------------------------------------------------------------------------#
SRC			= src/main.cpp src/Configuration.cpp $(CORESRCS) $(CONFSRCS) $(HTTPSRCS)

OBJ			= $(addprefix obj/, $(notdir $(SRC:.cpp=.o)))

################################################################################

all:		$(NAME)

$(NAME):	obj $(OBJ)
	@$(CXX) $(CXXFLAGS) $(INC) -o $(NAME) $(OBJ)
	@printf "\n\n\033[38;5;202m$(NAME) ready to run B)\033[0m\n"

obj:
	@mkdir obj

obj/%.o:	%.cpp
	@$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@
	@printf "\r\033[38;5;6m[$<]\033[0m Compiled!\n"
	@$(foreach file,$(shell ls obj/),printf '>';)

clean:
	@rm -rf obj
	@printf "\n\033[38;5;11mObjects folder has been taken care of Mx. President.\033[0m\n"

fclean:		clean
	@rm -rf $(NAME)
	@printf "\033[38;5;1mNo more \033[4m$(NAME).\033[0m\n\n"

re:			fclean all

.PHONY:		all fclean clean re

#////////////////////////////////////      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\#
