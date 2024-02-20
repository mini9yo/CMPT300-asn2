#include "printThread.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t threadPrint;
static pthread_cond_t s_printCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_printMutex = PTHREAD_MUTEX_INITIALIZER;

// Print thread implementation
void* printThread()
{
    // Wait until signal received
    pthread_mutex_lock(&s_printMutex);
    {
        pthread_cond_wait(&s_printCond, &s_printMutex);
    }
    pthread_mutex_unlock(&s_printMutex);

    // TODO: Update
    return NULL;
}

// Print signaller
void print_signal()
{
    pthread_mutex_lock(&s_printMutex);
    {
        pthread_cond_signal(&s_printCond);
    }
    pthread_mutex_unlock(&s_printMutex);
}

// Initialize printThread
void print_init()
{
    if (pthread_create(&threadPrint, NULL, printThread, NULL) != 0) {
        perror("Error threadPrint creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown printThread
void print_waitForShutdown()
{
    pthread_join(threadPrint, NULL);
}