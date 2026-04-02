CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g
LDFLAGS = -lreadline  -lgvc -lcgraph
BIN_DIR = build
LIBRARY_DIR = library
SRC_DIR = src

LIBRARY_SRC_FILES = $(SRC_DIR)/struct_private.c $(SRC_DIR)/struct_public.c
LIBRARY_OBJ_FILES = $(LIBRARY_DIR)/struct_private.o $(LIBRARY_DIR)/struct_public.o
LIBRARY_TARGET = $(LIBRARY_DIR)/libstruct.a

PROGRAM_SRC_FILES = $(SRC_DIR)/auxiliary.c $(SRC_DIR)/main.c $(SRC_DIR)/input_check.c $(SRC_DIR)/stack.c $(SRC_DIR)/timing.c $(SRC_DIR)/balance.c
PROGRAM_OBJ_FILES = $(BIN_DIR)/auxiliary.o $(BIN_DIR)/main.o $(BIN_DIR)/input_check.o $(BIN_DIR)/stack.o $(BIN_DIR)/timing.o $(BIN_DIR)/balance.o
PROGRAM_TARGET = $(BIN_DIR)/GO

all: library program

library: $(LIBRARY_TARGET)

program: $(PROGRAM_TARGET)

$(PROGRAM_TARGET): $(PROGRAM_OBJ_FILES) | $(LIBRARY_TARGET)
	$(CC) $(CFLAGS) $(PROGRAM_OBJ_FILES) -L$(LIBRARY_DIR) -lstruct -o $@ $(LDFLAGS)

$(LIBRARY_TARGET): $(LIBRARY_OBJ_FILES)
	@mkdir -p $(LIBRARY_DIR)
	ar rcs $@ $^

$(LIBRARY_DIR)/struct_private.o: $(SRC_DIR)/struct_private.c
	@mkdir -p $(LIBRARY_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBRARY_DIR)/struct_public.o: $(SRC_DIR)/struct_public.c
	@mkdir -p $(LIBRARY_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/auxiliary.o: $(SRC_DIR)/auxiliary.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/input_check.o: $(SRC_DIR)/input_check.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/stack.o: $(SRC_DIR)/stack.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/timing.o: $(SRC_DIR)/timing.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/balance.o: $(SRC_DIR)/balance.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean-library:
	rm -rf $(LIBRARY_DIR)/*

clean-program:
	rm -rf $(BIN_DIR)/*

clean: clean-library clean-program

.PHONY: all library program clean clean-library clean-program
