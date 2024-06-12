// "Client" dependencies
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

// Other dependencies
#include "message.h"
#include <stdbool.h>
#include <string.h>

#define JOIN_KEYWORD "JOIN"
#define LEAVE_KEYWORD "LEAVE"
#define SHUTDOWN_ALL_KEYWORD "SHUTDOWN ALL"
#define SHUTDOWN_KEYWORD "SHUTDOWN"

/*
Implements loop of listening for messages from server. Expects a pointer
to a socket as the argument.
Dependencies: parseMessage, writeMessageToSocket, scanf
*/
void* senderLoop(void* arg);

/*
Reads the string and attempts to interpret as a message, and writes data
to the given message structure. Returns true if successful, false if not
Dependencies: sscanf
*/
bool parseMessage(char* inString, Message* outMsg);
