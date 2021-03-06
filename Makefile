CC=gcc

NAME=shl
UNNAME=unhl

INC_DIR=include
SRC_DIR=src
OBJ_DIR=obj
BIN_DIR=bin
TEST_DIR=test

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

$(BIN_DIR)/$(NAME): $(OBJ) $(SRC_DIR)/$(NAME)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	strip -s $@

$(BIN_DIR)/$(UNNAME): $(SRC_DIR)/$(UNNAME)/main.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	strip -s $@

$(BIN_DIR)/%: $(TEST_DIR)/%.c $(OBJ) | $(BIN_DIR)
	#echo '$!' '$%' '$&' '$*' '$<' '$^' '$@'
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$* $^

compilemain: $(BIN_DIR)/$(NAME) $(BIN_DIR)/$(UNNAME)


build: compilemain

clean:
	rm -rfv bin obj
