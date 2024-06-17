#include "chat_node.h"

#define DBG
#include "dbg.h"

/*
  Function: addChatNodeToList

  Purpose: Adds a chatNode to a list
*/
void addChatNodeToList( ChatNodeList* chatNodeList, ChatNode* chatNode )
{
  // declare variables
  ChatNode *wkgPtr = chatNodeList -> firstPtr;

  // ensure there is an item
  if ( wkgPtr != NULL )
  {
    // iterate to end of list
    while (wkgPtr -> next != NULL)
    {
      wkgPtr = wkgPtr -> next;
    }
    // set last item -> next to chatNode
    wkgPtr->next = createChatNodeFromData(chatNode->ip, chatNode->port, chatNode->name);
    debug("Added user to linked list: %s\n", chatNode->name);

  }

  // list is empty
  else
  {
    debug("Added FIRST user to linked list: %s\n", chatNode->name);
    chatNodeList -> firstPtr = createChatNodeFromData(chatNode->ip, chatNode->port, chatNode->name);
  }

}

/*
  Function: clearChatNodeList

  Purpose: Uses helper function to clear nodes, then list struct
*/
ChatNodeList* clearChatNodeList( ChatNodeList *list )
{
  clearChatNodeListHelper( list -> firstPtr );
  free ( list );
  return NULL;
}

/*
  Function: clearChatNodeListHelper

  Purpose: Recursively clears nodes
*/
ChatNodeList* clearChatNodeListHelper( ChatNode *wkgPtr )
{
  // loop through each item in the LL
  if ( wkgPtr != NULL )
  {
   // recursively check the next item
   clearChatNodeListHelper( wkgPtr -> next );

   // the nodePtr's nextPtr = NULL so lets free this one too
   free( wkgPtr );
  }

  // return nothing
  return NULL;
}

/*
  Function: compareChatNodes

  Purpose: Compares two chat nodes, returns boolean success
*/
bool compareChatNodes( ChatNode* first, ChatNode* second )
{
  return ( first -> ip == second -> ip &&
            first -> port == second -> port &&
              privateCompareStrings( first -> name, second -> name ) == 0 );
}

/*
  Function: createChatNodeFromData

  Purpose: Takes client data and creates a node from it
*/
ChatNode* createChatNodeFromData( unsigned int ip, unsigned short int port,
                                                    char* name)
{
  // declare variables
  ChatNode *newNode;

  // allocate memory for newNode
  newNode = ( ChatNode* )malloc( sizeof( ChatNode ) );

  // set data pieces
  newNode -> ip = ip;
  newNode -> port = port;
  strncpy(newNode -> name, name, NAME_LEN);
  newNode -> next = NULL;

  return newNode; // return newNode
}

/*
  Function: deepCopyChatNode

  Purpose: Copies one node to another
*/
void deepCopyChatNode( ChatNode* destNode, ChatNode* srcNode)
{
  destNode -> ip = srcNode -> ip;
  destNode -> port = srcNode -> port;
  strncpy(destNode->name, srcNode->name, NAME_LEN);
  destNode -> next = srcNode -> next;
}

/*
  Function: displayLinkedList

  Purpose: Prints linked list to console
*/
void displayLinkedList( ChatNodeList* chatNodeList )
{
  // declare variables
  ChatNode* wkgPtr = chatNodeList -> firstPtr;

  // loop thru list
  while ( wkgPtr != NULL)
  {
    // print node
    printElement( wkgPtr );

    // print ptr
    if (wkgPtr -> next != NULL )
    {
      debug( "|\nv\n" );
    }

    wkgPtr = wkgPtr->next;
  }
}

/*
  Function: initializeChatNodeList

  Purpose: Initializes list of chat nodes
*/
ChatNodeList* initializeChatNodeList( void )
{
  // allocate memory for ChatNodeList struct
  ChatNodeList *newData;

  newData = ( ChatNodeList *)malloc( sizeof (ChatNodeList) );

  newData -> firstPtr = NULL;

  return newData;
}

void printElement( ChatNode *node)
{
  debug("Name: %s | Port: %u | IP: %u\n", node->name, node->port, node->ip);
}
/*
  Function: privateCompareStrings

  Purpose: compares two strings
*/
int privateCompareStrings( const char *leftStr, const char *rightStr )
{
  // compare strings
  int diff, index = 0;
  int leftStrLen = privateGetStringLen( leftStr );
  int rightStrLen = privateGetStringLen( rightStr );

  // begin while loop
  while( index < leftStrLen && index < rightStrLen )
  {
    diff = leftStr[ index ] - ( rightStr[ index ] );

    // return difference if different
    if( diff != 0 )
    {
      return diff;
    }

    index = index + 1;
  }
  return leftStrLen - rightStrLen;
}

/*
  Function: privateGetStringLen

  Purpose: Gets the length of a string
*/
int privateGetStringLen( const char *str )
{
 // declare variables
 int index = 0;

 // copies string character for character up to NULL_CHAR of source string
 while (str[index] != NULL_CHAR)
 {
  index = index + 1; // increment index
 }

 // return index
 return index;
}

/*
  Function: removeNodeFromList

  Purpose: Removes a single chatNode from a list, returns success bool
*/
bool removeNodeFromList( ChatNodeList* chatNodeList, ChatNode *chatNode)
{
  // declare variables
  ChatNode *temp, *parentNode;
  ChatNode *wkgPtr = chatNodeList -> firstPtr;

  // check for wkgPtr != NULL
  if ( wkgPtr != NULL )
  {
    // Special case: chatNode is first
    if ( compareChatNodes ( wkgPtr, chatNode ) )
    {
      // Set temp to chatNodeList -> first
      temp = chatNodeList -> firstPtr;

      // Set chatNodeList -> first to chatNodeList -> first -> next
      chatNodeList -> firstPtr = temp -> next;

      // clear temp
      free( temp );

      // return true
      return true;
    }

    // grab parent node
      // function: removeNodeFromListHelper
    parentNode = removeNodeFromListHelper( wkgPtr, chatNode );

    // if parentNode != NULL
    if ( parentNode != NULL )
    {
      // set temp to parentNode -> next
      temp = parentNode -> next;

      // set parentNode -> next = temp -> next
      parentNode -> next = temp -> next;

      // free temp
      free( temp );

      // return true
      return true;
    }
  }
  return false;
}

/*
  Function: removeNodeFromListHelper

  Purpose: Recursively tries to find parentNode for remove function
*/
ChatNode* removeNodeFromListHelper( ChatNode* wkgPtr, ChatNode *compareNode)
{
  // declare variables
  ChatNode *childPtr;

  // iterate through list
  if ( wkgPtr != NULL )
  {
    // grab childPtr
    childPtr = wkgPtr -> next;

    // short circuit, and compare child
    if ( childPtr != NULL && compareChatNodes( childPtr, compareNode ) )
    {
      return wkgPtr; // parent found
    }

    // check to move down one more
    else if ( childPtr != NULL)
    {
      return removeNodeFromListHelper( childPtr, compareNode );
    }

    // node not found! fall through
  }
  return NULL;
}
