#!/bin/bash

FOLDER_PATH=app/
gcc -Wall $FOLDER_PATH/server.c $FOLDER_PATH/properties.c -o testserver.o -pthread