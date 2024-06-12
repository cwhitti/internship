#!/bin/bash

# define global variables
DIR=remote
USERNAME=""
PROPERTIES_FILE=""
CLIENT_CONTAINER_NAME=""
CLIENT_IMAGE_NAME=""
BUILD_CLIENT=false
RUN_CLIENT=false
RUN_AS_AI=false
PORT=0

bold=$(tput bold)
reset=$(tput sgr0)

# define main script
main () {

    # declare variables
        # none

    # check for bad call
    check_empty "$@"

    # update global variables
    USERNAME=$1
    PROPERTIES_FILE="src/properties/${DIR}/${USERNAME}.properties"

    # check if file exists
    check_file

    # Iterate over arguments
    while [[ $# -gt 1 ]]; do

        # begin after 2nd arg
        case "$2" in
            -b) # build image
                BUILD_CLIENT=true
                ;;
            -r) # run image
                RUN_CLIENT=true
                ;;
            -ai) # run with AI
                RUN_AS_AI=true
                ;;
        esac
        shift # Shift arguments
    done #end while loop

    PORT=$(grep '^MY_PORT' "$PROPERTIES_FILE" | cut -d '=' -f 2 | tr -d '\r' | tr -d '\n' | tr -d ' ')

    # build the client
    if [ "$BUILD_CLIENT" = true ]; then
        docker_build
    fi 

    # run the client
    if [ $RUN_CLIENT = true ]; then
        docker_run
    fi 

    # run with AI
    if [ $RUN_AS_AI = true ]; then
     docker exec $CLIENT_IMAGE_NAME sh -c 'echo JOIN'
    fi

    exit 0
}

check_empty () 
{
    if [ $# -eq 0 ]; then
        usage
        exit 1
    fi

    if [ $# -eq 1 ]; then
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
    # name image
    CLIENT_IMAGE_NAME="client_image_$USERNAME"

    # build
    if ! docker build --build-arg USERNAME=$USERNAME --build-arg PORT=$PORT --build-arg DIR=$DIR -t $CLIENT_IMAGE_NAME . ; then
        echo "Failed to build $CLIENT_IMAGE_NAME"
        exit 1
    fi
    # send confirmation message
    echo "Built client image: $bold$CLIENT_IMAGE_NAME$reset"
}

docker_run () 
{
    # declare variables
    CLIENT_CONTAINER_NAME=client_container_$USERNAME
    CLIENT_IMAGE_NAME=client_image_$USERNAME

    echo "Trying to run $bold$CLIENT_CONTAINER_NAME$reset from $bold$CLIENT_IMAGE_NAME$reset..."

    docker run -it --name $CLIENT_CONTAINER_NAME -p $PORT:$PORT $CLIENT_IMAGE_NAME
    #if ! docker run -it --name $CLIENT_CONTAINER_NAME -p $PORT:$PORT $CLIENT_IMAGE_NAME; then

     #   container_id=$(docker ps -a | awk '/client_container_claire/ {print $1}')
        #docker ps -qf "name=client_container_claire"

      #  echo "(!) Container stopped suddenly - ID: $container_id?"
        
       # read -p "Do you want to delete the container (Y/N)? " choice

        #case "$choice" in
         #   y|Y ) docker rm "$container_id";;
          #  n|N ) echo "Container not deleted.";;
           # * ) echo "Invalid choice. Container not deleted.";;
        #esac
    #fi

}

usage () 
{
    echo "$bold Usage$reset: $0 <username> [-b] [-r] [-ai]
        "
    echo "$bold Options$reset:"
    echo "  -b        Build the image"
    echo "  -r        Run the client"
    echo "  -ai       Run client with AI"
}

# run the main script
main "$@"