#include "client_handler.h"

//#define DBG
#include "dbg.h"

// find client in list
bool clientInList( ChatNodeList *clientList, Message* messageObj )
{
  // declare variables
  ChatNode *wkgPtr = clientList -> firstPtr;
  ChatNode msgSender = messageObj -> messageSender;

  // loop thru list
  while ( wkgPtr != NULL && !compareChatNodes( wkgPtr, &msgSender ) )
  {
    wkgPtr = wkgPtr -> next;
  }

  return wkgPtr != NULL;

}
// join function
void clientJoin( ChatNodeList *clientList, Message* messageObj )
{
  // grab client from messageObj
  ChatNode clientNode = messageObj->messageSender;

  debug("Trying to add %s\n", clientNode.name);

  // add client to clientList
    // function: addChatNodeToList
  addChatNodeToList(clientList, &clientNode);

  // change msgtype from JOIN > JOINED
  messageObj->messageType = JOINING;
  strncpy( messageObj->noteContent, "User has joined!!!\n",
                                    sizeof(messageObj->noteContent));

  // send join message to all clients
    // function: forwardMessage()
  forwardMessage(clientList, messageObj);
}

// leave function
void clientLeave( ChatNodeList *clientList, Message* messageObj )
{
  // grab client from messageObj
  ChatNode clientNode = messageObj->messageSender;

  // remove chat node
    // function: removeNodeFromList
  if (removeNodeFromList(clientList, &clientNode))
  {
    debug("%s was removed!\n", clientNode.name);

    // change msgtype from LEAVE > LEAVING
    messageObj->messageType = LEAVING;
    strncpy( messageObj->noteContent, "User has Left!!!\n",
                                      sizeof(messageObj->noteContent));

    // send
      // function: forwardMessage()
    forwardMessage(clientList, messageObj);
  }
}

void forwardMessage( ChatNodeList *clientList, Message* messageObj )
{
  debug("Got into forward message\n");
  debug("Message to forward:");
  printMessageStruct(messageObj);
   // declare variables
  ChatNode* wkgPtr;

  // grab client from messageObj
  ChatNode clientNode = messageObj->messageSender;

  for (wkgPtr = clientList->firstPtr; wkgPtr != NULL; wkgPtr = wkgPtr->next)
  {
    if ( !compareChatNodes(wkgPtr, &clientNode) )
    {

      debug("Trying to forward to: ");
      printElement( wkgPtr );
      debug("\n");

      int sendSocket = socket(AF_INET, SOCK_STREAM, 0);;

      struct sockaddr_in clientAddress;
      clientAddress.sin_family = AF_INET;
      clientAddress.sin_addr.s_addr = htonl(wkgPtr->ip);
      clientAddress.sin_port = htons(wkgPtr->port);

      if (connect(sendSocket, (struct sockaddr *)&clientAddress,
                                                  sizeof(clientAddress)) == -1)
      {
        perror("Error forwarding to client");
        exit(EXIT_FAILURE);
      }

      debug("Connected to person\n");
      writeMessageToSocket(sendSocket, messageObj);
      debug("Successfully wrote to person\n");

      // disconnect
      if (close(sendSocket) == -1)
      {
          perror("Error closing socket");
          exit(EXIT_FAILURE);
      }

      debug("Closed socket to client\n");
      debug("Sent to %s\n", wkgPtr->name);
    }
  }
}

void* handle_client( void* args )
{
  // initialize variables
  ThreadArgs *threadArgs = (ThreadArgs *)args;

  // grab client socket
  int clientSocket = threadArgs->clientSocket;
  // grab clientList
  ChatNodeList* clientList = threadArgs->clientList;
  pthread_mutex_t* mainLock = threadArgs->mainLock;
  pthread_mutex_t* llLock = threadArgs->llLock;

  pthread_mutex_unlock(mainLock);
  pthread_mutex_unlock(llLock);

  debug("Client accepted!\n");

  // read entire message from socket, returns pointer to new msg struct
    // function: readMessageFromSocket( )
  Message* messageObj = readMessageFromSocket( clientSocket );

  // process depending on MessageType
  switch (messageObj->messageType)
  {
    //  JOIN
    case JOIN:

      if ( !clientInList( clientList, messageObj ) )
      {
        // function: clientJoin()
        debug("%s joined\n", messageObj->messageSender.name);
        clientJoin(clientList, messageObj);
        debug("Finsihed joining\n");
      }
      break;

    //  LEAVE
    case LEAVE:

      if ( clientInList( clientList, messageObj ) )
      {
        // function: clientLeave()
        debug("%s left\n", messageObj->messageSender.name);
        clientLeave(clientList, messageObj);
        debug("Finished leaving\n");
      }

      break;

    //  SHUTDOWN
    case SHUTDOWN:

      if ( clientInList( clientList, messageObj ) )
      {
        debug("%s shutdown\n", messageObj->messageSender.name);
        // function: clientLeave()
        clientLeave(clientList, messageObj);
        debug("Finished shutting down\n");
      }

      break;

    //  SHUTDOWN_ALL
    case SHUTDOWN_ALL:

      if ( clientInList( clientList, messageObj ) )
      {
        debug("%s shutdown all\n", messageObj->messageSender.name);
        // function: forwardMessage()
        forwardMessage(clientList, messageObj);

        // function: clearChatNodeList()
        clearChatNodeList(clientList);

        debug("Finished shutdown all\n");
      }

      exit(EXIT_SUCCESS);

      break;

    //  NOTE
    case NOTE:
      debug("Note from %s\n", messageObj->messageSender.name);
      // function: forwardMessage()

      if ( clientInList( clientList, messageObj ) )
      {
        forwardMessage(clientList, messageObj);
        debug("Finished forwarding the note\n");
      }
      else
      {
        debug("%s not in list\n", messageObj->messageSender.name);
      }

      break;

    default:
      break;
  }

  // debug purposes
  displayLinkedList( clientList );

  // exit thread
  pthread_exit(NULL);
}
