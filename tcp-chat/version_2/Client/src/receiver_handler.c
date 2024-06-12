#include "receiver_handler.h"

/*
Implements loop of listening for messages from server. Expects a pointer
to a socket as the argument.
Dependencies: readMessageFromSocket, printf
*/
void* receiverLoop(void* arg)
{
    // cast argument to ChatNode**, get chat nodes
    ChatNode** chatNodes = (ChatNode**)arg;
    ChatNode* myNode = chatNodes[0];
    ChatNode* serverNode = chatNodes[1];
    Message* inMsg;
    int yes = 1;

    // set up a socket with my chat node info and start listening
    struct sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    clientAddress.sin_port = htons(myNode->port);

    int receivingSocket;

    signal(SIGPIPE, SIG_IGN);

    // create the receiver socket
    if ((receivingSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // lose the pesky "Address already in use" error message
    if (setsockopt(receivingSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // bind receiving socket
    if (bind(receivingSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) != 0)
    {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // start listening on receiving socket
    if (listen(receivingSocket, NUM_CONNECTIONS) != 0)
    {
        perror("Error listening on socket");
        exit(EXIT_FAILURE);
    }

    // receiver loop
    while (true)
    {
        // accept a new connection from server
        int serverSocket = accept(receivingSocket, NULL, NULL);

        // read message from server
            // function: readMessageFromSocket
        inMsg = readMessageFromSocket(serverSocket);

        // process depending on message type:
        switch (inMsg->messageType)
        {
            // JOINED: print join message
                // function: printJoinMessage
            case JOINING:
            printJoinMessage(inMsg);
            break;

            // LEAVING: print leaving message
                // function: printLeaveMessage
            case LEAVING:
            printLeaveMessage(inMsg);
            break;

            // NOTE: print note
                // function: printNote
            case NOTE:
            printNote(inMsg);
            break;

            // SHUTDOWN: exit the receiver
                // function: pthread_exit
            case SHUTDOWN_ALL:
            exit(EXIT_SUCCESS);
            break;

            default:
            printf("You fucked up\n");
            break;
        }

        if (close(serverSocket) == -1)
        {
            perror("Error closing server socket");
            exit(EXIT_FAILURE);
        }
    }
}

void printNote(Message* inMsg)
{
    printf("%s: %s", inMsg->messageSender.name, inMsg->noteContent);
}

void printJoinMessage(Message* inMsg)
{
    printf("(!) %s joined the chat\n", inMsg->messageSender.name);
}

void printLeaveMessage(Message* inMsg)
{
    printf("(!) %s left the chat\n", inMsg->messageSender.name);
}
