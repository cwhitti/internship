#!/bin/bash

FOLDER_PATH=server/app
EXECUTABLE=testserver.o

if gcc -Wall $FOLDER_PATH/server1.c -o $EXECUTABLE -pthread; then
    echo "
    Successful compile: $EXECUTABLE
    "
fi