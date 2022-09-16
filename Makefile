CC = cc
INCLUDES = ./includes
FLAGS = -Wall -Werror -Wpedantic -Wextra -I$(INCLUDES)
SRC = ./src
OBJ = ./obj
BIN = ./bbsh
SRC_FILES = $(wildcard $(SRC)/*)
OBJ_FILES = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))
all: setup $(BIN) clean_dirs

$(OBJ)/main.o: $(SRC_FILES)
	$(CC) $(FLAGS) -c -o $@ $(SRC)/main.c

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(FLAGS) -c -o $@ $<

$(BIN): $(OBJ_FILES)
	$(CC) $(FLAGS) -o $@ $(OBJ_FILES)

debug: $(BIN)
	valgrind --log-file=valgrind.log ./bbsh

clean: clean_dirs clean_bin

clean_dirs:
	rm -rf $(OBJ) sample
clean_bin:
	rm -rf $(BIN)

setup:
	mkdir -p obj