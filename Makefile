NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter -std=c++98 -g

INC = -I include -I include/ConfigurationParser

SRC = src/main.cpp src/Cluster.cpp src/Server.cpp src/Port.cpp src/HTTPRequestParser.cpp src/Configuration.cpp \
	src/ConfigurationParser/ConfigurationParser.cpp src/ConfigurationParser/utils/_TempConfiguration.cpp \
	src/ConfigurationParser/utils/_TempRoute.cpp src/ConfigurationParser/utils/ParsingUtils.cpp src/HTTPResponse.cpp

OBJ = obj/main.o obj/Cluster.o obj/Server.o obj/Port.o obj/HTTPRequestParser.o obj/ConfigurationParser.o \
	obj/_TempConfiguration.o obj/_TempRoute.o obj/ParsingUtils.o obj/Configuration.o obj/HTTPResponse.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(INC) -o $(NAME) $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) -c $(SRC)
	rm -rf obj
	mkdir obj
	mv *.o obj

clean:
	rm -rf obj
	rm -rf *.o

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re
