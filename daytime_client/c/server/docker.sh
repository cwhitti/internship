#!/bin/bash

# define global variables
BUILD_CLIENT=false
RUN_CLIENT=false
DEBUG=false
CLEAN=false

CTR_NAME="server_container"
IMG_NAME="server_image"

PROPERTIES_FILE="properties/server.properties"
EXECUTABLE="./run_server"

PORT=0

bold=$(tput bold)
reset=$(tput sgr0)

# define main script
main () {

    # `d`eclare variables
        # none

    # check for bad call
    check_empty "$@"

    check_file

    # Iterate over arguments
    while [[ $# -gt 0 ]]; do

        # begin after 2nd arg
        case "$1" in
            -b) # build image
                BUILD_CLIENT=true

                ;;
            -r) # run image
                RUN_CLIENT=true
                ;;
            -db) # debug flag
                DEBUG=true
                ;;
            -c) # clean docker
                CLEAN=true

        esac
        shift # Shift arguments left
    done #end while loop

    # set port
    PORT=$(grep '^SERVER_PORT' "$PROPERTIES_FILE" | cut -d '=' -f 2 | tr -d '\r' | tr -d '\n' | tr -d ' ')
    # PORT=$(grep '^CLIENT_PORT' "$PROPERTIES_FILE" | cut -d '=' -f 2 | tr -d '\r' | tr -d '\n' | tr -d ' ')
    
    echo_variables

    # export debug command
    export DEBUG

    # clean existing stuff
    if [ $CLEAN = true ]; then
        docker_clean
        exit 0
    fi

    # build the client
    if [ "$BUILD_CLIENT" = true ]; then
        docker_build
    fi 

    # run the client
    if [ $RUN_CLIENT = true ]; then
        docker_run
    fi 

    exit 0
}

check_empty () 
{
    if [ $# -eq 0 ]; then
        usage
        exit 1
    fi
}

check_file ()
{
    # VULNERABILITY: CHECK IF FILE EXISTS
    if [ ! -f "$PROPERTIES_FILE" ]; then
        echo "Error: Property file '$PROPERTIES_FILE' does not exist."
        exit 1
    fi
}

docker_build ()
{
    echo docker build --build-arg EXECUTABLE=$EXECUTABLE \
                      --build-arg PROPERTY_FILE=$PROPERTIES_FILE \
                      --build-arg PORT=$PORT \
                      -t $IMG_NAME .
    # build
    if ! docker build --build-arg EXECUTABLE=$EXECUTABLE \
                      --build-arg PROPERTY_FILE=$PROPERTIES_FILE \
                      --build-arg PORT=$PORT \
                      -t $IMG_NAME . ; then
        echo "Failed to build $IMG_NAME"
        exit 1
    fi
}

docker_clean ()
{
    # Check if there are any containers to stop and remove
    if [ -n "$(docker ps -a -q)" ]; then
        echo "Stopping all running containers..."
        docker stop $(docker ps -a -q)  # Stop all containers

        echo "Removing all stopped containers..."
        docker rm $(docker ps -a -q)    # Remove all containers

            # Delete all images
        echo "Deleting all images..."
        docker rmi $(docker images -q)  # Delete all images
    else
        echo "Nothing to stop or remove."
    fi
}


docker_run () 
{
    echo "Trying to run $bold$CTR_NAME$reset from $bold$IMG_NAME$reset..."

    docker run -it --name $CTR_NAME -p $PORT:$PORT $IMG_NAME
}

echo_variables ()
{
    echo "CTR_NAME:       $CTR_NAME"
    echo "IMG_NAME:       $IMG_NAME"
    echo "PROPERTIES_FILE:$PROPERTIES_FILE"
    echo "EXECUTABLE:     $EXECUTABLE"
    echo "PORT:           $PORT"
}

usage () 
{
    echo "$bold Usage$reset: $0 <username> [-b] [-r] [-ai]
        "
    echo "$bold Options$reset:"
    echo "  -c        Clear all previous images and containers"
    echo "  -b        Build the image"
    echo "  -r        Run the server"
    echo "  -db       Run with debug"
}

# run the main script
main "$@"