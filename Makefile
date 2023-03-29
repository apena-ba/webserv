NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -Wno-unused -Wno-unused-parameter -std=c++98 -g

INC = -I include

SRC = src/main.cpp src/Server.cpp src/Clients.cpp

OBJ = obj/main.o obj/Server.o obj/Clients.o

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

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re