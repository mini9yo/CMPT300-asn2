#include "inputThread.h"
#include "receiveThread.h"
#include "printThread.h"
#include "sendThread.h"
#include "list.h"
#include "sockets.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 4) {
        fprintf(stderr, "Usage: %s [s-talk] [local_port] [remote_machine] [remote_port]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Parse command-line arguments
    int localPort = atoi(argv[1]);
    char *remoteMachine = argv[2];
    int remotePort = atoi(argv[3]);

    // Establish socket connection
    int socketDescriptor = createSocket(localPort);
    if (socketDescriptor < 0) {
        fprintf(stderr, "Error creating socket\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the list data structure for message passing
    List *messageListReceive = List_create();
    if (messageListReceive == NULL) {
        fprintf(stderr, "Error creating message list\n");
        closeSocket(socketDescriptor); // Close the socket
        exit(EXIT_FAILURE);
    }

    List *messageListSend = List_create();
    if (messageListSend == NULL) {
        fprintf(stderr, "Error creating message list\n");
        closeSocket(socketDescriptor); // Close the socket
        List_free(messageListReceive, free);
        exit(EXIT_FAILURE);
    }

    // Initialize threads
    input_init(messageListSend);
    send_init(messageListSend, socketDescriptor, remoteMachine, remotePort);
    receive_init(messageListReceive, socketDescriptor);
    print_init(messageListReceive);


    // Wait for threads to finish
    input_shutdown();
    send_shutdown();
    receive_shutdown();
    print_shutdown();

    // Clean up resources
    List_free(messageListSend, free);
    List_free(messageListReceive, free);
    closeSocket(socketDescriptor);

    return EXIT_SUCCESS;
}
