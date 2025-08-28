# makefile
# Comprehansive make file for all compiles of modules
# created in the project/coding refresher

# The compiler: 'gcc' for C program, 'g++' for C++ program
CC = gcc

# Compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall


# The build targets - executables:
EXE1 = ./helloworld
EXE2 = ./hello2
EXE3 = ./hello3
EXE4 = ./timestrings
# Make a list of all the .exe's that should be built as necessary
EXE = $(EXE1) $(EXE2) $(EXE3) $(EXE4) 

# Make directive: Build all where the target (executable) is missing
# This just directs to the 'all' label by default
default: all

#Individual build rules:
# Build individual if the source has changed 
$(EXE1): $(EXE1).c
		$(CC) $(CFLAGS) -o $(EXE1) $(EXE1).c

# Build individual if the source has changed
$(EXE2): $(EXE2).c
		$(CC) $(CFLAGS) -o $(EXE2) $(EXE2).c

# Build individual if the source has changed
$(EXE3): $(EXE3).c
		$(CC) $(CFLAGS) -o $(EXE3) $(EXE3).c

# Build individual if the source has changed
$(EXE4): $(EXE4).c
		$(CC) $(CFLAGS) -o $(EXE4) $(EXE4).c

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
		$(RM) $(EXE1)
		$(RM) $(EXE2)
		$(RM) $(EXE3)
		$(RM) $(EXE4)