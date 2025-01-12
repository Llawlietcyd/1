
# Compiler and flags
CC = gcc
CFLAGS = -std=c99 -pedantic -Wreturn-type -Wimplicit-function-declaration

# Define the executable file name
EXECUTABLE = lab

# Object files needed to create the executable
OBJECTS = labmain.o lab.o

# Default target: compile everything
all: $(EXECUTABLE)

# Link the object files to create the final executable
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECUTABLE)

# Compile lab4main.c to an object file
lab4main.o: labmain.c lab.h
	$(CC) $(CFLAGS) -c labmain.c -o labmain.o

# Compile lab4.c to an object file
lab4.o: lab.c lab.h
	$(CC) $(CFLAGS) -c lab.c -o lab.o

# Clean up: remove object files and the executable
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Archive the source files and Makefile into a zip
archive: Makefile labmain.c lab.c lab.h
	zip lab_archive.zip Makefile labmain.c lab.c lab.h
