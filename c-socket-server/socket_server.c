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
#define SERVER_VERSION "1.0.0" // Define server version

int server_fd;
DLT_DECLARE_CONTEXT(dlt_ctx); // Declare dlt_ctx at the global scope
volatile sig_atomic_t stop_server = 0; // Flag to indicate server should stop

void handle_signal(int signal) {
    const char *signal_str;
    switch (signal) {
        case SIGINT:
            signal_str = "SIGINT";
            break;
        case SIGTERM:
            signal_str = "SIGTERM";
            break;
        default:
            signal_str = "UNKNOWN";
    }
    DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Received signal: "), DLT_STRING(signal_str));
    if (signal == SIGINT || signal == SIGTERM) { // Handle both SIGINT and SIGTERM
        DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Shutting down server..."));
        stop_server = 1; // Set flag to stop server
    }
}

int main(int argc, char const *argv[]) {
    DLT_REGISTER_APP("SRK", "SRK Socket Server");
    DLT_REGISTER_CONTEXT(dlt_ctx, "SRKS", "SRK Socket Server Context");

    DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Starting SRK Socket Server version: "), DLT_STRING(SERVER_VERSION)); // Log server version

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

    int bind_attempts = 5;
    while (bind_attempts > 0) {
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
            DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Bind failed, retrying..."));
            bind_attempts--;
            sleep(1); // Wait for 1 second before retrying
        } else {
            break;
        }
    }

    if (bind_attempts == 0) {
        DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Bind failed after multiple attempts"));
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
    signal(SIGTERM, handle_signal); // Catch SIGTERM signal

    while (!stop_server) {
        DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Waiting for a connection..."));
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            if (stop_server) break; // Exit loop if stopping
            DLT_LOG(dlt_ctx, DLT_LOG_ERROR, DLT_STRING("Accept failed"));
            return -1;
        }

        while (!stop_server) {
            int valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread <= 0 || stop_server) { // Exit read loop if stopping
                DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Client disconnected or server stopping"));
                break;
            }
            buffer[valread] = '\0';
            DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Message from Client: "), DLT_STRING(buffer));
            send(new_socket, hello, strlen(hello), 0);
            DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Hello message sent"));
        }

        close(new_socket);
    }
    DLT_LOG(dlt_ctx, DLT_LOG_INFO, DLT_STRING("Server Shutdown Complete"));
    close(server_fd);
    DLT_UNREGISTER_CONTEXT(dlt_ctx);
    DLT_UNREGISTER_APP();
    return 0;
}