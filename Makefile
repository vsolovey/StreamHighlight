CC=gcc
CFLAGS=-O2

#SRC=$(wildcard src/*.c)
#OBJ=$(SRC:src%.c=obj%.o)
OBJ_DIR=obj
BIN_DIR=bin

default: build

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

directories: $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

compile: $(OBJ_DIR)/main.o 
	$(CC) -o $(BIN_DIR)/emph $^

build: | directories compile

clean:
	rm -rfv bin obj
