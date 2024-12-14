#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "socket_server.h"

#define SERVER_IP "192.168.100.22"
#define SERVER_PORT 9874
#define BUFFER_SIZE 1024

int main() {
    int server_socket = create_socket();
    bind_socket(server_socket, SERVER_IP, SERVER_PORT);
    listen_for_connections(server_socket);

    while (1) {
        int client_socket = accept_connection(server_socket);
        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}