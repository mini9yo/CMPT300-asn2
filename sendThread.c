#include "sendThread.h"
#include <pthread.h>
#include <stdlib.h>

pthread_t threadSend;
static pthread_cond_t s_sendCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_sendMutex = PTHREAD_MUTEX_INITIALIZER;

// Send thread implementation
void* sendThread()
{
    // Wait until signal received
    pthread_mutex_lock(&s_sendMutex);
    {
        pthread_cond_wait(&s_sendCond, &s_sendMutex);
    }
    pthread_mutex_unlock(&s_sendMutex);

    // TODO: Update
    return NULL;
}

// Send signaller
void send_signal()
{
    pthread_mutex_lock(&s_sendMutex);
    {
        pthread_cond_signal(&s_sendCond);
    }
    pthread_mutex_unlock(&s_sendMutex);
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