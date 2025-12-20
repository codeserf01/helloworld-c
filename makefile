# makefile
# Comprehansive make file for all compiles of modules
# created in the project/coding refresher

# The compiler: 'gcc' for C program, 'g++' for C++ program
CC = gcc

# Compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

BIN  = ./bin/
SRC  = ./src/

# The build targets - executables:
EXE01  = $(BIN)helloworld
EXE02  = $(BIN)hello2
EXE03  = $(BIN)hello3
EXE04  = $(BIN)timestrings
EXE10  = $(BIN)base_template
EXE20  = $(BIN)msglog

# The source codes
SRC01  = $(SRC)helloworld
SRC02  = $(SRC)hello2
SRC03  = $(SRC)hello3
SRC04  = $(SRC)timestrings
SRC10  = $(SRC)base_template
SRC20  = $(SRC)msglog

# Make a list of all the .exe's that should be built as necessary
EXE = $(EXE01) $(EXE02) $(EXE03) $(EXE04) $(EXE10) $(EXE20) 

# Make directive: Build all where the target (executable) is missing
# This just directs to the 'all' label by default
default: all

#Individual build rules:
# Build the individual module if its source has changed 
$(EXE01): $(SRC01).c
		$(CC) $(CFLAGS) -o $(EXE01) $(SRC01).c

$(EXE02): $(SRC02).c
		$(CC) $(CFLAGS) -o $(EXE02) $(SRC02).c

$(EXE03): $(SRC03).c
		$(CC) $(CFLAGS) -o $(EXE03) $(SRC03).c

$(EXE04): $(SRC04).c
		$(CC) $(CFLAGS) -o $(EXE04) $(SRC04).c

$(EXE10): $(SRC10).c
		$(CC) $(CFLAGS) -o $(EXE10) $(SRC10).c

$(EXE20): $(SRC20).c
		$(CC) $(CFLAGS) -o $(EXE20) $(SRC20).c

#-----------------------------------------------------------
# Build all where the exe is missing
# EXE is a list of executables/targets
# As specified above, 'all' is the default

all: $(EXE)
	@echo
	@echo make targets : "$?"
	@echo

# No build - delete/cleanup
clean: 
		$(RM) $(EXE01)
		$(RM) $(EXE02)
		$(RM) $(EXE03)
		$(RM) $(EXE04)
		$(RM) $(EXE10)
		$(RM) $(EXE20)