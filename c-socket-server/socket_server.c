#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#define PORT 9876
#define BUFFER_SIZE 1024

int server_fd;

void handle_signal(int signal) {
    if (signal == SIGINT) {
        printf("\nShutting down server...\n");
        close(server_fd);
        exit(0);
    }
}

int main(int argc, char const *argv[]) {
    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *hello = "Hello from server";

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <interface>\n", argv[0]);
        return -1;
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return -1;
    }

    // Forcefully attaching socket to the specified interface and port 9876
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        return -1;
    }

    signal(SIGINT, handle_signal);

    while (1) {
        printf("Waiting for a connection...\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            return -1;
        }

        while (1) {
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread <= 0) {
                printf("Client disconnected\n");
                break;
            }
            buffer[valread] = '\0';
            printf("Message from client: %s\n", buffer);
            send(new_socket, hello, strlen(hello), 0);
            printf("Hello message sent\n");
        }

        close(new_socket);
    }

    close(server_fd);
    return 0;
}