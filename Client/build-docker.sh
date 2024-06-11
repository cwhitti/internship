#!/bin/bash

RUN=false

if [ $# -eq 0 ]; then
    echo "Usage: $0 <username> <run>"
    exit 1
fi


USERNAME=$1

PROPERTIES_FILE="src/properties/remote/${USERNAME}.properties"

# VULNERABILITY: CHECK IF FILE EXISTS
if [ ! -f "$PROPERTIES_FILE" ]; then
    echo "Error: Property file '$PROPERTIES_FILE' does not exist."
    exit 1
fi

echo "building image: chat_client_image_$USERNAME" 

docker build --build-arg USERNAME=$USERNAME -t chat_client_image_$USERNAME .

if [ "$2" = true ]; then
    echo "Running client..."
    docker run --name chat_client_container_$USERNAME -p 5001:5001 chat_client_image_$USERNAME
fi