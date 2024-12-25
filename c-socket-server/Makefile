CC = gcc
CFLAGS = -Iinclude -Wall
SRC = socket_server.c
OBJ = $(SRC:.c=.o)
TARGET = srk-socket-server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)