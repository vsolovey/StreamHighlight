CC=gcc

NAME=emph

INC_DIR=include
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin

CFLAGS=-O2 -I$(INC_DIR)

SRC=$(wildcard $(SRC_DIR)/*.c)
OBJ=$(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

default: build

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(BIN_DIR)/$(NAME): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(NAME) $^

compilemain: $(BIN_DIR)/$(NAME)


build: compilemain

clean:
	rm -rfv bin obj
