CC = gcc
CFLAGS = -Iinclude -Wall
SRC = src/socket_server.c
OBJ = $(SRC:.c=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)