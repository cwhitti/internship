// "Server" dependencies
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <netdb.h>

#include "message.h"

#ifndef NUM_CONNECTIONS
#define NUM_CONNECTIONS 20
#endif

/*
Implements loop of listening for messages from server. Expects a pointer
to a socket as the argument.
Dependencies: readMessageFromSocket, printf
*/
void* receiverLoop(void* arg);

/*
Prints a message like "name: note" for a NOTE message type
Dependencies: printf
*/
void printNote(Message* inMsg);

/*
Prints a message like "(!) <name> joined the chat" for a JOINED
message type
Dependencies: printf
*/
void printJoinMessage(Message* inMsg);

/*
Prints a message like "(!) <name> left the chat" for a LEAVING
message type
Dependencies: printf
*/
void printLeaveMessage(Message* inMsg);
