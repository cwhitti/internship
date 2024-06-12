#include "main.h"

/* ************************************************************************* */
/* MAIN                                                                      */
/* ************************************************************************* */

int main(int argc, char** argv)
{
     printf("HIELLOxxxx");
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    ThreadArgs threadArgs; // create thread args
    ChatNodeList* clientList; // TODO: initialize data for clientList
    int yes = 1;
    int port;
    Properties* properties;

    printf("HIELLO");

    properties = property_read_properties(argv[1]);
    sscanf(property_get_property(properties, "SERVER_PORT"), "%d", &port);
    debug("Listening on port: %d\n", port);

    pthread_mutex_t mainLock = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t llLock = PTHREAD_MUTEX_INITIALIZER;

    clientList = initializeChatNodeList();

    // assign clientList to threadArgs
    threadArgs.clientList = clientList;
    threadArgs.mainLock = &mainLock;
    threadArgs.llLock = &llLock;

    // ----------------------------------------------------------
    // ignore SIGPIPE, sent when client disconnected
    // ----------------------------------------------------------
    signal(SIGPIPE, SIG_IGN);

    // ----------------------------------------------------------
    // create unnamed network socket for server to listen on
    // ----------------------------------------------------------
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // lose the pesky "Address already in use" error message
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // ----------------------------------------------------------
    // bind the socket
    // ----------------------------------------------------------
    server_address.sin_family      = AF_INET;           // accept IP addresses
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept clients on any interface
    server_address.sin_port        = htons(port);       // port to listen on

    // binding unnamed socket to a particular port
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) != 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // ----------------------------------------------------------
    // listen on the socket
    // ----------------------------------------------------------
    if (listen(server_socket, NUM_CONNECTIONS) != 0)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&mainLock, NULL);
    pthread_mutex_init(&llLock, NULL);
    pthread_mutex_lock(&mainLock);
    pthread_mutex_lock(&llLock);

    // ----------------------------------------------------------
    // server loop
    // ----------------------------------------------------------
    while (TRUE)
    {
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // assign clientSocket to threadArgs
        threadArgs.clientSocket = client_socket;

        // create thread to handle the client's request
        // note that this is a naive approach, i.e. there are race conditions
        // for now this is okay, assuming low load
          // FUNCTION: handle_client
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void*)&threadArgs) != 0)
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&mainLock);
        pthread_mutex_lock(&llLock);

        displayLinkedList( clientList );

        // detach the thread so that we don't have to wait (join) with it to reclaim memory.
        // memory will be reclaimed when the thread finishes.
        if (pthread_detach(thread) != 0)
        {
            perror("Error detaching thread");
            exit(EXIT_FAILURE);
        }
    }
}
