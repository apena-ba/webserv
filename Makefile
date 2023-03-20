NAME = webserv

CXX = c++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRC = src/main.cpp

OBJ = obj/main.o

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJ)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)
	rm -rf obj
	mkdir obj
	mv *.o obj

clean:
	rm -rf obj

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all fclean clean re