#include "server.h"
#define DBG

/* ************************************************************************* */
/* MAIN                                                                      */
/* ************************************************************************* */

int main(int argc, char** argv)
{
    int server_socket;                 // descriptor of server socket
    struct sockaddr_in server_address; // for naming the server's listening socket
    int yes = 1;

    // add-ons
    int port;
    Properties* properties;

    // set properties
    properties = property_read_properties( "server/app/properties/server.properties" );
    sscanf(property_get_property(properties, "SERVER_PORT"), "%d", &port);

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

    printf( "Listening on %d:%d\n", server_address.sin_addr.s_addr, port);

    // ----------------------------------------------------------
    // server loop
    // ----------------------------------------------------------
    while (TRUE)
    {
        printf("Listening for new client...\n");
        
        // accept connection to client
        int client_socket = accept(server_socket, NULL, NULL);
        printf("\nServer with PID %d: accepted client\n", getpid());

        // create thread to handle the client's request
        // note that this is a naive approach, i.e. there are race conditions
        // for now this is okay, assuming low load
        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, (void*)&client_socket) != 0)
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }

        // detach the thread so that we don't have to wait (join) with it to reclaim memory.
        // memory will be reclaimed when the thread finishes.
        if (pthread_detach(thread) != 0)
        {
            perror("Error detaching thread");
            exit(EXIT_FAILURE);
        }
    }
}


/* ************************************************************************* */
/* handle client                                                             */
/* ************************************************************************* */

void* handle_client(void* arg)
{
    int client_socket = *((int*)arg);   // the socket connected to the client
    char  outStr[HUGE_STR_LEN];

    // get time
    get_current_time( outStr );

    // send result back to client
    write(client_socket, &outStr, sizeof(outStr));

    // cleanup
    if (close(client_socket) == -1)
    {
        perror("Error closing socket");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Closed socket to client, exit\n");
    }

    pthread_exit(NULL);
}

void get_current_time( char *outStr )
{
  // declare variables
  time_t rawtime;
  struct tm *info;
  char buffer[HUGE_STR_LEN];

  // get the time
  time(&rawtime);

  // ensure rawtime
  if (rawtime == -1)
  {
      perror("Error getting current time");
      exit(EXIT_FAILURE);
  }

  // get localtime
  info = localtime(&rawtime);

  // ensure localtime
  if (info == NULL)
  {
      perror("Error converting time");
      exit(EXIT_FAILURE);
  }

  // throw time into buffer
    // ensure format
  if (strftime(buffer, HUGE_STR_LEN, "%A, %B %d, %Y %H:%M:%S %Z", info) == 0)
  {
      perror("Error formatting time");
      exit(EXIT_FAILURE);
  }

  // copy buffer into outStr
  strncpy(outStr, buffer, HUGE_STR_LEN);
}