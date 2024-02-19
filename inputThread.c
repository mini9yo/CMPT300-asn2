#include "inputThread.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t threadInput;

// Input thread implementation
void* inputThread()
{
    // TODO: Update
    return NULL;
}

// Initialize inputThread
void input_init()
{
    if (pthread_create(&threadInput, NULL, inputThread, NULL) != 0) {
        perror("Error threadInput creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown inputThread
void input_waitForShutdown()
{
    pthread_join(threadInput, NULL);
}