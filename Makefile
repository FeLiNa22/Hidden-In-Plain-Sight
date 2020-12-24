# Directories
SRC_DIR = src/
LIB_DIR = lib/
INC_DIR = inc/
OBJ_DIR = obj/
DBG_DIR = debug/

# C Compiler options
CC 	   = gcc
CFLAGS = -Wall -g 
IFLAGS = -I$(INC_DIR)
LFLAGS = -L$(LIB_DIR)

# Files to use in compilation
OBJCTS = bmp.o decode.o encode.o main.o utils.o encrypt.o encrypt_utils.o

# Target output
TARGET = steg

all: $(TARGET)

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

test: 
	sh test.sh

test-clean: 
	rm -rf $(DBG_DIR)/*.out $(DBG_DIR)/*.txt

# Final output requires no changes
$(TARGET) : $(addprefix $(OBJ_DIR), $(OBJCTS))
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) -o $@ $^

# Compiles all src files into objects
obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(IFLAGS) $(LFLAGS) -c $^ -o $@ 

.PHONY: test
