#!/bin/bash

# set variables
RUN=false
DIR=remote

if [ $# -eq 0 ]; then
    echo "Usage: $0 <username> <bool:run>"
    exit 1
fi

USERNAME=$1

PROPERTIES_FILE="src/properties/${DIR}/${USERNAME}.properties"

# VULNERABILITY: CHECK IF FILE EXISTS
if [ ! -f "$PROPERTIES_FILE" ]; then
    echo "Error: Property file '$PROPERTIES_FILE' does not exist."
    exit 1
fi

export PORT=$(grep '^MY_PORT' "$PROPERTIES_FILE" | cut -d '=' -f 2 | tr -d '\r' | tr -d '\n' | tr -d ' ')

docker build --build-arg USERNAME=$USERNAME --build-arg PORT=$PORT --build-arg DIR=$DIR -t client_image_$USERNAME .

if [ "$2" = true ]; then
    echo "Running client..."
    docker run -it --name client_container_$USERNAME -p $PORT:$PORT client_image_$USERNAME 
fi