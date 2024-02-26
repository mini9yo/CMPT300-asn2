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
    List *messageList = List_create();
    if (messageList == NULL) {
        fprintf(stderr, "Error creating message list\n");
        exit(EXIT_FAILURE);
    }

    // Initialize threads
    input_init(messageList);
    receive_init(messageList, localPort);
    print_init(messageList);
    send_init(messageList, remoteMachine, remotePort);

    // Wait for threads to finish
    input_waitForShutdown();
    receive_waitForShutdown();
    print_waitForShutdown();
    send_waitForShutdown();

    // Clean up resources
    List_free(messageList, free);

    return EXIT_SUCCESS;
}
