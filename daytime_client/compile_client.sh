#!/bin/bash

FOLDER_PATH=client/app
EXECUTABLE=testclient.o

if gcc -Wall $FOLDER_PATH/client.c $FOLDER_PATH/properties.c -o $EXECUTABLE; then
    echo "
    Successful compile: $EXECUTABLE
    "
fi