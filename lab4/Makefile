
# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -pedantic -Wreturn-type -Wimplicit-function-declaration

# Define the executable file name
EXECUTABLE = lab4

# Object files needed to create the executable
OBJECTS = lab4main.o lab4.o

# Default target: compile everything
all: $(EXECUTABLE)

# Link the object files to create the final executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

# Compile lab4main.c to an object file
lab4main.o: lab4main.c lab4.h
	$(CC) $(CFLAGS) -c lab4main.c -o lab4main.o

# Compile lab4.c to an object file
lab4.o: lab4.c lab4.h
	$(CC) $(CFLAGS) -c lab4.c -o lab4.o

# Clean up: remove object files and the executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Archive the source files and Makefile into a zip
archive: Makefile lab4main.c lab4.c lab4.h
	zip lab4_archive.zip Makefile lab4main.c lab4.c lab4.h
