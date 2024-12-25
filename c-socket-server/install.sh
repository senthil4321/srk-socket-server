#!/bin/bash

# Install script for the server program

# Compile the server program
make

# Check if the compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed. Please check the Makefile and source code."
    exit 1
fi

# Create a directory for the server program if it doesn't exist
INSTALL_DIR="/usr/local/bin/server-program"
if [ ! -d "$INSTALL_DIR" ]; then
    sudo mkdir -p "$INSTALL_DIR"
fi

# Copy the compiled executable to the installation directory
sudo cp ./src/socket_server "$INSTALL_DIR"

# Set the appropriate permissions
sudo chmod +x "$INSTALL_DIR/socket_server"

echo "Server program installed successfully in $INSTALL_DIR."