#\\\\\\\\\\\\\\\\\\\\\\\  NTAMAYO-'s Amazing Makefile  ////////////////////////#

NAME		= webserv

CXX			= c++

CXXFLAGS	= -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter -std=c++98

INC			= -I include -I include/ConfigurationParser

VPATH		= ./src:./src/ConfigurationParser/:./src/ConfigurationParser/utils/

#----                                                                      ----#

CORESRCS	= $(addprefix src/, Cluster.cpp Server.cpp Port.cpp)
CONFSRCS	= $(addprefix src/ConfigurationParser/, ConfigurationParser.cpp utils/_TempConfiguration.cpp utils/_TempRoute.cpp utils/ParsingUtils.cpp)
HTTPSRCS	= $(addprefix src/, HTTPRequestParser.cpp HTTPResponse.cpp)
#------------------------------------------------------------------------------#
SRC			= src/main.cpp src/Configuration.cpp $(CORESRCS) $(CONFSRCS) $(HTTPSRCS)

OBJ			= $(addprefix obj/, $(notdir $(SRC:.cpp=.o)))

################################################################################

all:		$(NAME)

$(NAME):	obj $(OBJ)
	@mv *.o obj/
	@$(CXX) $(CXXFLAGS) $(INC) -o $(NAME) $(OBJ)
	@printf "\n\033[38;5;202m$(NAME) ready to run B)\033[0m\n"

obj:
	@mkdir obj

obj/%.o:	%.cpp
	@$(CXX) $(CXXFLAGS) $(INC) -c $<
	@printf "\033[38;5;6m[$<]\033[0m Compiled!\n"

clean:
	@rm -rf obj
	@printf "\n\033[38;5;11mObjects folder has been taken care of Mx. President.\033[0m\n"

fclean:		clean
	@rm -rf $(NAME)
	@printf "\033[38;5;1mNo more \033[4m$(NAME).\033[0m\n\n"

re:			fclean all

.PHONY:		all fclean clean re

#////////////////////////////////////      \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\#



# NAME = webserv
#
# CXX = c++
#
# CXXFLAGS = -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter -std=c++98 -g
#
# INC = -I include -I include/ConfigurationParser
#
# SRC = src/main.cpp src/Cluster.cpp src/Server.cpp src/Port.cpp src/HTTPRequestParser.cpp src/Configuration.cpp \
#     src/ConfigurationParser/ConfigurationParser.cpp src/ConfigurationParser/utils/_TempConfiguration.cpp \
#     src/ConfigurationParser/utils/_TempRoute.cpp src/ConfigurationParser/utils/ParsingUtils.cpp src/HTTPResponse.cpp
#
# OBJ = obj/main.o obj/Cluster.o obj/Server.o obj/Port.o obj/HTTPRequestParser.o obj/ConfigurationParser.o \
#     obj/_TempConfiguration.o obj/_TempRoute.o obj/ParsingUtils.o obj/Configuration.o obj/HTTPResponse.o
#
# all: $(NAME)
#
# $(NAME): $(OBJ)
#     $(CXX) $(CXXFLAGS) $(INC) -o $(NAME) $(OBJ)
#
# $(OBJ): $(SRC)
#     $(CXX) $(CXXFLAGS) $(INC) -c $(SRC)
#     rm -rf obj
#     mkdir obj
#     mv *.o obj
#
# clean:
#     rm -rf obj
#     rm -rf *.o
#
# fclean: clean
#     rm -rf $(NAME)
#
# re: fclean all
#
# .PHONY: all fclean clean re
