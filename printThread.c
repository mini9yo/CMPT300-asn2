#include "printThread.h"
#include "list.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "listOps.h"

// Initialize variables
static pthread_t threadPrint;
static pthread_cond_t s_printCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_printMutex = PTHREAD_MUTEX_INITIALIZER;

// Print thread implementation
void* printThread(void* listRx)
{
    char* msg;

    while (1) {
        // Wait until signal received
        pthread_mutex_lock(&s_printMutex);
        {
            pthread_cond_wait(&s_printCond, &s_printMutex);
        }
        pthread_mutex_unlock(&s_printMutex);

        // Remove message from list
        msg = listRemove(listRx);

        // Check for exit code ('!')
<<<<<<< HEAD
        if ((strlen(msg) == 1) && (*msg == '!')) {
=======
        if ((strlen(msg) == 1) && (msg[0] == '!')) {
>>>>>>> 2be837e128123a1064baf993d96530fd4b241258
            break;
        }

        // Print then free message
<<<<<<< HEAD
        puts(msg);
        free(msg);
        msg = NULL;
=======
        if (msg != NULL) {
            puts(msg);
            free(msg);
            msg = NULL;
        }
        
>>>>>>> 2be837e128123a1064baf993d96530fd4b241258
    }

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
void print_init(List* listRx)
{
    if (pthread_create(&threadPrint, NULL, printThread, (void*) listRx) != 0) {
        perror("Error threadPrint creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown printThread
<<<<<<< HEAD
void print_waitForShutdown()
=======
void print_shutdown()
>>>>>>> 2be837e128123a1064baf993d96530fd4b241258
{
    pthread_cancel(threadPrint);
    pthread_join(threadPrint, NULL);
}