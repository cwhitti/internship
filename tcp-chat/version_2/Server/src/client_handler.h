#include "chat_node.h"
#include "message.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <netdb.h>
#include <time.h>
#include <string.h>

// structs
typedef struct ThreadArgsStruct
{
    int clientSocket;
    ChatNodeList *clientList;
    pthread_mutex_t* mainLock;
    pthread_mutex_t* llLock;
} ThreadArgs;

// check if client in list
bool clientInList( ChatNodeList *clientList, Message* messageObj );

// join function
void clientJoin( ChatNodeList *clientList, Message* messageObj );

// leave function
void clientLeave( ChatNodeList *clientList, Message* messageObj );

// note function
void clientNote( ChatNodeList *clientList, Message* messageObj );

// general forward message function
void forwardMessage( ChatNodeList *clientList, Message* messageObj );

// joining function
void globalJoining( ChatNodeList *clientList, Message* messageObj );

// leaving function
void globalLeaving( ChatNodeList *clientList, Message* messageObj );

// shutdownAll
void globalShutdown( ChatNodeList *clientList, Message* messageObj );

void* handle_client( void* args );
