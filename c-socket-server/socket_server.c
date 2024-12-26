#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <dlt/dlt.h> // Include DLT logging

#define PORT 9876
#define BUFFER_SIZE 1024

int server_fd;
DLT_DECLARE_CONTEXT(dlt_ctx); // Declare dlt_ctx at the global scope

void handle_signal(int signal) {
    if (signal == SIGINT) {
        DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Shutting down server..."));
        close(server_fd);
        DLT_UNREGISTER_APP();
        exit(0);
    }
}

int main(int argc, char const *argv[]) {
    DLT_REGISTER_APP("SRK", "SRK Socket Server");
    DLT_REGISTER_CONTEXT(dlt_ctx, "SRKS", "SRK Socket Server Context");

    int new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *hello = "Hello from server";

    if (argc != 2) {
        DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Usage: %s <interface>"), argv[0]);
        return -1;
    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Socket creation failed"));
        return -1;
    }

    // Forcefully attaching socket to the specified interface and port 9876
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Bind failed"));
        close(server_fd);
        DLT_UNREGISTER_CONTEXT(dlt_ctx);
        DLT_UNREGISTER_APP();
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Listen failed"));
        return -1;
    }

    signal(SIGINT, handle_signal);

    while (1) {
        DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Waiting for a connection..."));
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Accept failed"));
            return -1;
        }

        while (1) {
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread <= 0) {
                DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Client disconnected"));
                break;
            }
            buffer[valread] = '\0';
            DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Message from client: %s"), buffer);
            send(new_socket, hello, strlen(hello), 0);
            DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Hello message sent"));
        }

        close(new_socket);
    }

    close(server_fd);
    DLT_UNREGISTER_CONTEXT(dlt_ctx);
    DLT_UNREGISTER_APP();
    return 0;
}