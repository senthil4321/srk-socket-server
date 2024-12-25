#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9876
#define MESSAGE "Hello from client"
#define INTERVAL 3 // 3 seconds

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if (argc != 2) {
        printf("Usage: %s <server_address>\n", argv[0]);
        return -1;
    }

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        if (send(sock, MESSAGE, strlen(MESSAGE), 0) == -1) {
            printf("Failed to send message. Server might have closed the connection.\n");
            break;
        }
        printf("Message sent to server\n");
        if (read(sock, buffer, 1024) <= 0) {
            printf("Failed to read message. Server might have closed the connection.\n");
            break;
        }
        printf("Message from server: %s\n", buffer);
        sleep(INTERVAL);
    }

    close(sock);
    return 0;
}