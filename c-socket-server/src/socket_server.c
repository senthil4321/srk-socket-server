#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include "socket_server.h"

#define BUFFER_SIZE 1024

int server_socket;

void handle_shutdown(int sig) {
    printf("Shutting down server...\n");
    close(server_socket);
    exit(EXIT_SUCCESS);
}

void start_server(const char *server_ip, int server_port) {
    int client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Register signal handler for graceful shutdown
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);

    // Create a TCP/IP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    // Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on %s:%d\n", server_ip, server_port);

    while (1) {
        // Wait for a connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        printf("Connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Receive data from the client
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Receive failed");
            close(client_socket);
            continue;
        }
        buffer[bytes_received] = '\0'; // Null-terminate the received data
        printf("Received data: %s\n", buffer);

        // Send a response back to the client
        const char *response = "Data received";
        send(client_socket, response, strlen(response), 0);

        // Close the connection
        close(client_socket);
    }

    // Close the server socket (unreachable code in this example)
    close(server_socket);
}