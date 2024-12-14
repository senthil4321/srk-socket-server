#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include <netinet/in.h>

#define SERVER_PORT 9874
#define BUFFER_SIZE 1024

void start_server(const char *ip_address);
void handle_client(int client_socket);
int create_socket();
void bind_socket(int server_socket, struct sockaddr_in *server_addr);
void listen_for_connections(int server_socket);
int accept_connection(int server_socket, struct sockaddr_in *client_addr);
void receive_data(int client_socket, char *buffer, size_t buffer_size);
void send_response(int client_socket, const char *response);

#endif // SOCKET_SERVER_H