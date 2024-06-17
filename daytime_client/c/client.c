/*
PA2

Claire Whittington
Peter Hilbert

*/

#include "server.h"

/************************************************************************
 * MAIN
 ************************************************************************/
int main()
{
  int client_socket;                  // client side socket
  struct sockaddr_in client_address;  // client socket naming struct
  char ip_addr[INET_ADDRSTRLEN];
  char inString[HUGE_STR_LEN];

  // get IP address of the common name server
  get_ip_address( SERVER_ADDR, ip_addr );

  // create an unnamed socket, and then name it
  client_socket = socket(AF_INET, SOCK_STREAM, 0);

  // create addr structhiii sxadsafasd
  client_address.sin_family = AF_INET;
  client_address.sin_addr.s_addr = inet_addr(ip_addr);
  client_address.sin_port = htons(PORT);

  // connect to server socket
  if (connect(client_socket, (struct sockaddr *)&client_address,
                                            sizeof(client_address)) == -1)
  {
    perror("Error connecting to server!\n");
    exit(EXIT_FAILURE);
  }

  // get the result
  read( client_socket, &inString, sizeof(inString) );

  // print to console
  printf( "%s\n", inString );

  return EXIT_SUCCESS;
}

void get_ip_address(const char *string, char *ip_string)
{
  struct addrinfo *headPtr, *wkgPtr;
  struct sockaddr_in *ipv4 = NULL;
  void *addr;
  char temp[INET_ADDRSTRLEN];

  //hints.ai_family = AF_INET; // AF_INET means IPv4 only

  if ( getaddrinfo( string, NULL, NULL, &headPtr ) == 0 )
  {
    // Loop through all the results and get the IP address
    for (wkgPtr = headPtr; wkgPtr != NULL; wkgPtr = wkgPtr->ai_next)
    {
      ipv4 = (struct sockaddr_in *)wkgPtr->ai_addr;
      addr = &( ipv4->sin_addr );

      // Convert the IP to a string
      inet_ntop( wkgPtr->ai_family, addr, temp, sizeof temp);
    }
    // Free the linked list
    freeaddrinfo( headPtr );

    // Copy the IP address string
    strncpy(ip_string, temp, INET_ADDRSTRLEN);
  }
  else
  {
    perror("Error resolving nostname.");
    exit(EXIT_FAILURE);
  }
 }
