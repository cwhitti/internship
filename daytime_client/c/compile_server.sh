#!/bin/bash

# C source files
SOURCE_FILES=(
    server.c
)

# Output executable name
OUTPUT_NAME="run_server"

# Compile using GCC
if gcc -Wall "${SOURCE_FILES[@]}" -o "$OUTPUT_NAME" -pthread; then
  echo "Successful compile - output file named '$OUTPUT_NAME'".
else
  echo "Compilation failed".
fi
