# makefile
# Test make file for running a compile of helloworld.c

# the compiler: gcc for C program, g++ for C++ program
CC = gcc


# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall


# the build target executable:
TARGET = helloworld

# Build if the target (executable) is missing
all: $(TARGET)
		$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

# Build if the source has changed
$(TARGET): $(TARGET).c
		$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

# No build - delete/cleanup
clean: 
		$(RM) $(TARGET)