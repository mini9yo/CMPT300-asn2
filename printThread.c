#include "printThread.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t threadPrint;

// Print thread implementation
void* printThread()
{
    // TODO: Update
    return NULL;
}

void print_init()
{
    if (pthread_create(&threadPrint, NULL, printThread, NULL) != 0) {
        perror("Error threadPrint creation");
        exit(EXIT_FAILURE);
    }
}

void print_waitForShutdown()
{
    pthread_join(threadPrint, NULL);
}