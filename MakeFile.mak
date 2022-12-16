
CC = gcc
CFLAGS = -Wall -Wextra -Werror


SOURCES = tictactoe.c mqtt_pub.c mqtt_sub.c daemon.c
EXECUTABLE = tictactoe

# Specify the library dependencies
LIBS = -lmqtt

# Default target to build the executable
all: $(SOURCES)
    $(CC) $(CFLAGS) $(SOURCES) $(LIBS) -o $(EXECUTABLE)

clean:
    rm -f $(EXECUTABLE)

# Target to run the game
run: all
    ./$(EXECUTABLE)
