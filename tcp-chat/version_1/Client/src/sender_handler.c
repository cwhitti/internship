#include "sender_handler.h"
#define RUN_AS_AI
/*
Implements loop of listening for messages from server. Expects a pointer
to a socket as the argument.
Dependencies: parseMessage, writeMessageToSocket, scanf
*/
void* senderLoop(void* arg)
{
    // cast argument to ChatNode**
    // initialize variables
    ChatNode** argsPtr = (ChatNode** )arg;

    // extract my chat node and the server chat node
    ChatNode* clientNode = argsPtr[0];
    ChatNode* serverNode = argsPtr[1];
    FILE *filePointer;

    struct sockaddr_in serverAddress;
    int sendingSocket;
    Message msgStrct;

    // set up serverAddress
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(serverNode -> ip);
    serverAddress.sin_port = htons(serverNode -> port);

    // set message chat node to my chat node
        // function: deepCopyChatNode
    deepCopyChatNode( &msgStrct.messageSender, clientNode );
    msgStrct.messageType = NOTE;

    printf("SERVER: Hi, %s! Type JOIN to begin chatting.\n", clientNode->name);
    
    // loop until message is a SHUTDOWN or SHUTDOWN_ALL
    while ( msgStrct.messageType !=  SHUTDOWN &&
            msgStrct.messageType !=  SHUTDOWN_ALL )
    {
        #ifdef RUN_AS_AI
        filePointer = fopen("src/input.txt", "r");

        if (filePointer == NULL) 
        {
        printf("Error opening file!\n");
        }
        fgets(msgStrct.noteContent, NOTE_LEN, filePointer );
        fclose(filePointer);

        #else
        fgets(msgStrct.noteContent, NOTE_LEN, stdin );
        
        #endif
        // write data from string to message struct, check for success
            // function: parseMessage
        parseMessage( msgStrct.noteContent, &msgStrct );

        sendingSocket = socket(AF_INET, SOCK_STREAM, 0);

        // connect sending socket to server socket
        if (connect(sendingSocket, (struct sockaddr *)&serverAddress,
                                                            sizeof(serverAddress)) == -1)
        {
            perror("Error connecting to server!\n");
            exit(EXIT_FAILURE);
        }

            // if successful, write message to the socket
                // function: writeMessageToSocket
        writeMessageToSocket( sendingSocket, &msgStrct);
            //printf("Wrote message!\n");

        if (close(sendingSocket) == -1)
        {
            perror("Error closing socket!\n");
            exit(EXIT_FAILURE);
        }
    }

    exit(EXIT_SUCCESS);
}

/*
Reads the string and attempts to interpret as a message, and writes data
to the given message structure, specifically the message type and note contents.
Returns true if successful, false if not
Dependencies: sscanf
*/
bool parseMessage(char* inString, Message* outMsg)
{
    // check for JOIN keyword in first word
    if ( strncmp(JOIN_KEYWORD, inString, strlen(JOIN_KEYWORD)) == 0 )
    {
            // update messageType of outMsg
            outMsg -> messageType = JOIN;
    }
    // check for LEAVE keyword in first word
    else if (strncmp(LEAVE_KEYWORD, inString, strlen(LEAVE_KEYWORD)) == 0)
    {
        // update messageType of outMsg
        outMsg -> messageType = LEAVE;
    }

    // check for SHUTDOWN ALL
    else if (strncmp(SHUTDOWN_ALL_KEYWORD, inString,
                                        strlen(SHUTDOWN_ALL_KEYWORD)) == 0)
    {
        // update messageType of outMsg
        outMsg -> messageType = SHUTDOWN_ALL;
    }

    // check for SHUTDOWN
    else if (strncmp(SHUTDOWN_KEYWORD, inString, strlen(SHUTDOWN_KEYWORD)) == 0)
    {
        // update messageType of outMsg
        outMsg -> messageType = SHUTDOWN;
    }

    // otherwise, assume message is a NOTE
    else
    {
        // update messageType of outMsg
        outMsg -> messageType = NOTE;

        // update Note field of outMsg
            // function: strcpy
            strncpy( outMsg -> noteContent, inString, sizeof(outMsg->noteContent));
    }

    return true;
}
