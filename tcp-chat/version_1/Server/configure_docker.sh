#!/bin/bash

PROPERTIES_FILE="src/properties/server.properties"

PORT=$(grep '^SERVER_PORT' "$PROPERTIES_FILE" | cut -d '=' -f 2 | tr -d '\r' | tr -d '\n' | tr -d ' ')

echo "PORT is $PORT"

#if ! docker build --build-arg USERNAME=$USERNAME --build-arg PORT=$PORT -t $CLIENT_IMAGE_NAME . ; then
docker build --build-arg PORT=$PORT -t server_image .

docker run -it --name server_container -p $PORT:$PORT server_image
