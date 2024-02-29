#include "printThread.h"
#include "list.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initialize variables
static pthread_t threadPrint;
static pthread_cond_t s_printCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t s_printMutex = PTHREAD_MUTEX_INITIALIZER;

// Print thread implementation
void* printThread(void* threadArg)
{
    char* msg;
    List* listRx = threadArg;

    while (1) {
        // Wait until signal received
        pthread_mutex_lock(&s_printMutex);
        {
            pthread_cond_wait(&s_printCond, &s_printMutex);
        }
        pthread_mutex_unlock(&s_printMutex);

        // Remove message from list
        msg = List_remove(listRx);

        // Handle msg
        if (msg == NULL) {
            perror("Error: Attempted to remove item from empty list");
            exit(EXIT_FAILURE);
            
        } else { // Print message
            puts(msg);
            fflush(stdout); // flush to immediately display msg
            
            // Check for exit code ('!')
            if ((strlen(msg) == 1) && (*msg == '!')) {
                free(msg);
                msg = NULL;
                break;
            }
            
            free(msg);
            msg = NULL;
        }
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

// Cancel printThread
void cancelPrint()
{
    pthread_cancel(threadPrint);
}

// Shutdown printThread
void print_shutdown()
{
    pthread_join(threadPrint, NULL);
}