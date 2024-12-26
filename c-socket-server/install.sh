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
INSTALL_DIR="/usr/local/bin/srk-server"
if [ ! -d "$INSTALL_DIR" ]; then
    sudo mkdir -p "$INSTALL_DIR"
fi

SERVICE_NAME="srk-socket-server"

# Check if the service is running
if systemctl is-active --quiet $SERVICE_NAME; then
    echo "Stopping $SERVICE_NAME service..."
    sudo systemctl stop $SERVICE_NAME
fi

# Copy new files (assuming the new executable is in the current directory)
echo "Deploying new version of $SERVICE_NAME..."
sudo cp ./srk-socket-server /usr/local/bin/
# Set the appropriate permissions
sudo chmod +x "$INSTALL_DIR/$SERVICE_NAME"

# Start the service again
echo "Starting $SERVICE_NAME service..."
sudo systemctl start $SERVICE_NAME

# Check the status of the service
sudo systemctl status $SERVICE_NAME


echo "Server program installed successfully in $INSTALL_DIR."