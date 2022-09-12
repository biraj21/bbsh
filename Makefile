CC = cc
FLAGS = -Wall -Werror -Wpedantic -Wextra
SRC = ./src
OBJ = ./obj
BIN = ./bbsh
SRC_FILES = $(wildcard $(SRC)/*)
OBJ_FILES = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(wildcard $(SRC)/*.c))

all: setup clean $(BIN)

$(OBJ)/main.o: $(SRC_FILES)
	$(CC) $(FLAGS) -c -o $@ $(SRC)/main.c

$(OBJ)/%.o: $(SRC)/%.c $(SRC)/%.h
	$(CC) $(FLAGS) -c -o $@ $<

$(BIN): $(OBJ_FILES)
	$(CC) $(FLAGS) -o $@ $(OBJ_FILES)

debug: $(BIN)
	valgrind --log-file=valgrind.log ./bbsh

clean:
	rm -f $(BIN) $(OBJ)/*.o sample

setup:
	mkdir -p src obj