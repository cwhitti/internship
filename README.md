# TCP-Based Commandline Chat Application
This codebase serves as a case study for the AFRL. A basic demonstration is linked [here](https://www.youtube.com/watch?v=U73MY2SoXKI).

## Use
Once the server has been started, clients can begin connecting to a basic chatroom. All clients have a `properties` file (ex. claire.properties) which denote the server IP, server port, client IP, and client port, as well as the client username. Clients can connect through the terminal with`./build/chat_client src/client_properties/<PROPERTY FILE>`. 

Once the client establishes a connection, the client can send commands to the server. The server recognizes the following command-line commands:

  - **JOIN**: Connects client to the chat
  - **LEAVE**: Client leaves the chat but still stay connected to the server
  - **SHUTDOWN**: Client leaves the chat and disconnects from server
  - **SHUTDOWN_ALL**: Client shutsdown the entire server, ideally would be admin-only but it's not right now.
  - **NOTE**: Default command, any message sent that is not one of the former commands

Unused Commands
  - **JOINING/LEAVING**: Currently not used, but could be for some more complex UI

## Structure Overview
Both client and server can communicate with eachother using TCP protocol. The client and server have shared structures which are read and recieved.

  - **ChatNode**: A struct to identify chat participants (chat_node_struct)
    ```c
    // ChatNode w/ IP, Port, logical name
    typedef struct chat_node_struct
    {
      unsigned int ip;
      unsigned short int port;
      char name[ NAME_LEN ]; // 16 bytes
      struct chat_node_struct* next;
    } ChatNode;
    ```
  
  - **ChatNodeList**: A linked list struct to keep track of all chat participants
    ```c
    typedef struct chat_node_list_struct
    {
      ChatNode* firstPtr;
    
    } ChatNodeList;
    ```

  - **Message**: A struct which both client and server use to communicate
    ```c
    typedef char Note[NOTE_LEN];
    
    typedef struct messageStruct {
        MessageType messageType;
        ChatNode messageSender;
        Note noteContent; // char array, 64 bytes
    } Message;
    ```
 
  - **MessageType**: Enumerated values for all commands
    ```c
    typedef enum {
        JOIN = 0,
        LEAVE = 1,
        SHUTDOWN = 2,
        SHUTDOWN_ALL = 3,
        NOTE = 4,
        JOINING = 5,
        LEAVING = 6
    } MessageType;
    ```
