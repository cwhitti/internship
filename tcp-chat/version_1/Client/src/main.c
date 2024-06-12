#include "main.h"

//#define DBG
#include "dbg.h"

int main(int argc, char** argv)
{
    int receivingSocket, sendingSocket;
    pthread_t receivingThread, sendingThread;

    struct sockaddr_in clientAddress;
    char clientIPString[INET_ADDRSTRLEN];
    int myPort;
    char myName[NAME_LEN];

    struct sockaddr_in serverAddress;
    char serverIPString[INET_ADDRSTRLEN] ;
    int serverPort;
    char* serverName = "SERVER";

    Properties* properties;
    ChatNode** chatNodes = (ChatNode**)malloc( sizeof(ChatNode*) * 2 );

    // read properties
    if (argc != 2)
    {
        debug("Must include properties file name in arguments!\n");
        exit(EXIT_FAILURE);
    }

    properties = property_read_properties(argv[1]);

    // get string properties
    strncpy(clientIPString, property_get_property(properties, "MY_IP"), INET_ADDRSTRLEN);
    strncpy(serverIPString, property_get_property(properties, "SERVER_IP"), INET_ADDRSTRLEN);
    strncpy(myName, property_get_property(properties, "MY_NAME"), NAME_LEN);

    // get int properties
    sscanf(property_get_property(properties, "MY_PORT"), "%d", &myPort);
    sscanf(property_get_property(properties, "SERVER_PORT"), "%d", &serverPort);

    // set chat node array
    chatNodes[0] = createChatNodeFromData((unsigned int)ntohl(inet_addr(clientIPString)),
                                          (unsigned short int)myPort,
                                           myName);

    chatNodes[1] = createChatNodeFromData((unsigned int)ntohl(inet_addr(serverIPString)),
                                          (unsigned short int)serverPort,
                                           serverName);

    /***************
     Start receiver
    ****************/

    // start listening for messages from server
    pthread_create(&receivingThread, NULL, receiverLoop, (void*)chatNodes);

    /*************
     Start sender
    **************/

    // start sending messages to server
    senderLoop((void*)chatNodes);
}
