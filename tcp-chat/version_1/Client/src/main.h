#include <pthread.h>

#include "receiver_handler.h"
#include "sender_handler.h"
#include "properties.h"
#include "chat_node.h"

#define NUM_CONNECTIONS 20

typedef struct senderArgsStruct
{
    int sendingSocket;
    ChatNode* myChatNode;
} SenderArgs;