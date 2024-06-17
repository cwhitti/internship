#!/bin/bash

# C source files
SOURCE_FILES=(
    client.c
    properties.c
)

# Output executable name
OUTPUT_NAME="run_client"

# Compile using GCC
if gcc -Wall "${SOURCE_FILES[@]}" -o "$OUTPUT_NAME"; then
    echo "Successful compile - output file named '$OUTPUT_NAME'".
else
    echo "Compilation failed".
fi
