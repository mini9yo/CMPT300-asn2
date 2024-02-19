#include "sendThread.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t threadSend;

// Send thread implementation
void* sendThread()
{
    // TODO: Update
    return NULL;
}

// Initialize sendThread
void send_init()
{
    if (pthread_create(&threadSend, NULL, sendThread, NULL) != 0) {
        perror("Error threadSend creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown sendThread
void send_waitForShutdown()
{
    pthread_join(threadSend, NULL);
}