# c-socket-server/c-socket-server/README.md

# C Socket Server

This project implements a simple socket server in C. The server listens for incoming TCP connections, receives data from clients, and sends a response back.

## Project Structure

```
c-socket-server
├── src
│   ├── main.c          # Entry point of the application
│   └── socket_server.c  # Implementation of socket server functions
├── include
│   └── socket_server.h  # Declarations of functions and constants
├── Makefile             # Build rules for compiling the project
└── README.md            # Documentation for the project
```

## Building the Project

To build the project, navigate to the project directory and run the following command:

```bash
make
```

This will compile the source files and create an executable.

## Running the Server

After building the project, you can run the server using the following command:

```
./socket_server
```

Make sure to adjust the server IP and port in the source code as needed.

### [README.md](vscode-remote://wsl/home/srk/project/srk-socket-server/README.md)

Add the service activation procedure to the README file.

```markdown
# SRK Socket Server

## Overview
This project contains a TCP socket server and client implementation.

## Installation

### Server Installation
1. Run the install script to compile and install the server program:
    ```bash
    ./install.sh
    ```

2. The server program will be installed to `/usr/local/bin/server-program`.

### Client Installation
1. Compile the client program:
    ```bash
    gcc -o socket_client socket_client.c
    ```

## Running the Server
To run the server manually, use the following command:
```bash
/usr/local/bin/server-program/socket_server 0.0.0.0
```

## Running the Client

To run the client, use the following command:

```bash
./socket_client <server_address>
```

## Service Activation

To activate the server as a systemd service, follow these steps:

1. Copy the systemd service file to the system directory:

    ```bash
    sudo cp ./socket_server.service /etc/systemd/system/
    ```

2. Reload systemd to recognize the new service:

    ```bash
    sudo systemctl daemon-reload
    ```

3. Enable the service to start on boot:

    ```bash
    sudo systemctl enable socket_server.service
    ```

4. Start the service:

    ```bash
    sudo systemctl start socket_server.service
    ```

5. Check the status of the service:

    ```bash
    sudo systemctl status socket_server.service
    ```

## Stopping the Service

To stop the server service, use the following command:

```bash
sudo systemctl stop socket_server.service
```

## Disabling the Service

To disable the server service from starting on boot, use the following command:

```bash
sudo systemctl disable socket_server.service
```

## Dependencies

This project requires a standard C compiler and the socket library, which is typically available on Unix-like systems.

## License

This project is licensed under the MIT License. See the LICENSE file for more details.