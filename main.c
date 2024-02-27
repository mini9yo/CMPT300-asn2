// #include "list.h"
// #include "sockets.h"

// int main(int argc, char** args)
// {
//     // Initialize arguments
//     int myPort = args[1];
//     char *remoteMachine = args[2];
//     int remotePort = args[3];

//     // Establish socket connection
//     int socketDescriptor = createSocket(myPort);

//     // Create list variables
//     List *listRx = List_create(); // Storing inbound messages
//     List *listSx = List_create(); // Storing outbound messages

//     // Intialize pthreads
//     input_init(listSx);
//     send_init(listSx);
//     receive_init(listRx, socketDescriptor);
//     print_init(listRx);

//     // Cleanup pthreads
//     input_waitForShutdown();
//     send_waitForShutdown();
//     receive_shutdown();
//     print_shutdown();

//     // Close socket
//     closeSocket(socketDescriptor);

//     // TODO: Free list

//     return 0;
// }

#include "inputThread.h"
#include "receiveThread.h"
#include "printThread.h"
#include "sendThread.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [local_port] [remote_machine] [remote_port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse command-line arguments
    int localPort = atoi(argv[1]);
    char *remoteMachine = argv[2];
    int remotePort = atoi(argv[3]);

    // Initialize the list data structure for message passing
    List *messageListReceive = List_create();
    if (messageListReceive == NULL) {
        fprintf(stderr, "Error creating message list\n");
        exit(EXIT_FAILURE);
    }
  
    List *messageListSend = List_create();
    if (messageListSend == NULL) {
        fprintf(stderr, "Error creating message list\n");
        exit(EXIT_FAILURE);
    }

    // Initialize threads
    input_init(messageListSend);
    receive_init(messageListReceive, localPort);
    print_init(messageListReceive);
    send_init(messageListSend, remoteMachine, remotePort);

    // Wait for threads to finish
    input_waitForShutdown();
    receive_waitForShutdown();
    print_waitForShutdown();
    send_waitForShutdown();

    // Clean up resources
    List_free(messageListReceive, free);
     List_free(messageListSend, free);

    return EXIT_SUCCESS;
}

