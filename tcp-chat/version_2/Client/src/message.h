#ifndef MESSAGE_H
#define MESSAGE_H

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

#include "chat_node.h"

#define NOTE_LEN 64
#define NULL_CHAR '\0'

typedef enum {
    JOIN = 0,
    LEAVE = 1,
    SHUTDOWN = 2,
    SHUTDOWN_ALL = 3,
    NOTE = 4,
    JOINING = 5,
    LEAVING = 6
} MessageType;

typedef char Note[NOTE_LEN];

typedef struct messageStruct {
    MessageType messageType;
    ChatNode messageSender;
    Note noteContent;
} Message;

/*
Returns a pointer to a new message, initialized with given data
Dependencies: createChatNodeFromChatNode
*/
Message* createMessageFromData(MessageType inMsgType, 
                               ChatNode* inMsgSender,
                               Note inNoteContent);

/*
Returns a pointer to a new message, initialized with data
from another message
Dependencies: createMessageFromData
*/
Message* createMessageFromMessage(Message* inMsg);

/*
Reads data from the socket, expecting message data, and returns
pointer to new message
Dependencies: read, createMessageFromData, createChatNodeFromData
*/
Message* readMessageFromSocket(int socket);

void printMessageStruct(Message* msg);

/*
Reads data from the socket, expecting message data, and returns
pointer to new message
Dependencies: write, createMessageFromData, createChatNodeFromData
*/
void writeMessageToSocket(int socket, Message* outMsg);

/*
Helper function to ensure complete reads
Dependencies: read
*/
int completeRead(int socket, void* buffer, unsigned int size);

#endif