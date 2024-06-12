#include "message.h"

//#define DBG
#include "dbg.h"

/*
Returns a pointer to a new message, initialized with given data
Dependencies: createChatNodeFromChatNode
*/
Message* createMessageFromData(MessageType inMsgType,
                               ChatNode* inMsgSender,
                               Note inNoteContent)
{
    Message* newMessage = (Message*)malloc(sizeof(Message));
    newMessage->messageType = inMsgType;
    deepCopyChatNode(&(newMessage->messageSender), inMsgSender);
    strncpy((char*)&(newMessage->noteContent), (char*)inNoteContent, NOTE_LEN);
    newMessage->noteContent[NOTE_LEN - 1] = NULL_CHAR;

    return newMessage;
}

/*
Returns a pointer to a new message, initialized with data
from another message
Dependencies: createMessageFromData
*/
Message* createMessageFromMessage(Message* inMsg)
{
    return createMessageFromData(inMsg->messageType,
                                 &(inMsg->messageSender),
                                 inMsg->noteContent);
}

void printMessageStruct(Message* msg)
{
    debug("Type: %d , IP: %u , Port: %u , Name: %s, Note contents: %s\n",
            msg->messageType, msg->messageSender.ip, msg->messageSender.port, msg->messageSender.name, 
            msg->noteContent);
}

/*
Reads data from the socket, expecting message data, and returns
pointer to new message, returns NULL if there was a network error
Dependencies: completeRead, createMessageFromData, createChatNodeFromData
*/
Message* readMessageFromSocket(int socket)
{
    unsigned int inMsgType;
    unsigned int inIp;
    unsigned short int inPort;
    char inName[NAME_LEN];
    Note inNote;

    ChatNode* newChatNode;
    Message* newMessage;

    // read all raw data
    completeRead(socket, (void*)&inMsgType, sizeof(inMsgType));
    completeRead(socket, (void*)&inIp, sizeof(inIp));
    completeRead(socket, (void*)&inPort, sizeof(inPort));
    completeRead(socket, (void*)inName, NAME_LEN);
    completeRead(socket, (void*)inNote, NOTE_LEN);

    // convert raw data into data structures
    newChatNode = createChatNodeFromData(ntohl(inIp), ntohs(inPort), inName);
    newMessage = createMessageFromData(ntohl(inMsgType), newChatNode, inNote);

    debug("Message read:\n");
    printMessageStruct(newMessage);

    // return new message
    return newMessage;
}

/*
Helper function to ensure complete reads
Dependencies: read
*/
int completeRead(int socket, void* buffer, unsigned int size)
{
    ssize_t bytesRead = 0;
    ssize_t bytesReadAll = 0;
    char* bufferPtr = buffer;

    do
    {
        bytesRead = read(socket, bufferPtr, size);

        if (bytesRead == -1)
        {
            // network error
            return -1;
        }
        else
        {
            size -= (int)bytesRead;
            bufferPtr += bytesRead;
            bytesReadAll += bytesRead;
        }
    } while (size != 0);

    return (int)bytesReadAll;
}

/*
Reads data from the socket, expecting message data, and returns
pointer to new message
Dependencies: write
*/
void writeMessageToSocket(int socket, Message* outMsg)
{
    // apply network macros to appropriate values
    unsigned int outMsgType = htonl(outMsg->messageType);
    unsigned int outIp = htonl(outMsg->messageSender.ip);
    unsigned short int outPort = htons(outMsg->messageSender.port);

    // write message type
    write(socket, &(outMsgType), sizeof(outMsgType));

    // write chat node info
    write(socket, &(outIp), sizeof(outIp));
    write(socket, &(outPort), sizeof(outPort));
    write(socket, &(outMsg->messageSender.name), NAME_LEN);

    // write note content
    write(socket, &(outMsg->noteContent), NOTE_LEN);

    debug("Message written:\n");
    printMessageStruct(outMsg);
}
