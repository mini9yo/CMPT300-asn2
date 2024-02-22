#include "list.h"
#include "sockets.h"

int main(int argc, char** args)
{
    // Initialize arguments
    int myPort = args[1];
    char *remoteMachine = args[2];
    int remotePort = args[3];

    // Establish socket connection
    int socketDescriptor = createSocket(myPort);

    // Create list variables
    List *listRx = List_create(); // Storing inbound messages
    List *listSx = List_create(); // Storing outbound messages

    // Intialize pthreads
    input_init(listSx);
    send_init(listSx);
    receive_init(listRx, socketDescriptor);
    print_init(listRx);

    // Cleanup pthreads
    input_waitForShutdown();
    send_waitForShutdown();
    receive_shutdown();
    print_shutdown();

    // Close socket
    closeSocket(socketDescriptor);

    // TODO: Free list

    return 0;
}