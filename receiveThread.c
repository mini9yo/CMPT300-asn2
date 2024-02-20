#include "receiveThread.h"
#include "printThread.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t threadReceive;

// Receive thread implementation
void* receiveThread()
{
    // TODO: Update

    // Signal print thread
    print_signal();
    
    return NULL;
}

// Initialize receiveThread
void receive_init()
{
    if (pthread_create(&threadReceive, NULL, receiveThread, NULL) != 0) {
        perror("Error threadReceive creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown receiveThread
void receive_waitForShutdown()
{
    pthread_join(threadReceive, NULL);
}