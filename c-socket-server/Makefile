CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -ldlt
SRC = socket_server.c
OBJ = $(SRC:.c=.o)
TARGET = srk-socket-server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)