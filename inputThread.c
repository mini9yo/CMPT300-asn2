#include "inputThread.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "list.h"

#define BUFFER_MAX_LEN 256

static pthread_t threadInput;
static List* inputList;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t inputCond = PTHREAD_COND_INITIALIZER;
static int inputShutdown = 0;

// Input thread implementation
void* inputThread()
{
    // TODO: Update
    while (1) {
        char inputBuffer[BUFFER_MAX_LEN];
        fgets(inputBuffer, sizeof(inputBuffer), stdin);

        pthread_mutex_lock(&inputMutex);
        if (List_prepend(inputList, strdup(inputBuffer)) != 0) {
            perror("List add failed\n");
            exit(EXIT_FAILURE);
        }
        pthread_cond_signal(&inputCond);
        pthread_mutex_unlock(&inputMutex);

        if (strcmp(inputBuffer, "!\n") == 0) {
            inputShutdown = 1;
            break;
        }
    }

    return NULL;
}

// Initialize inputThread
void input_init(List* list)
{
    inputList = list;
    
    if (pthread_create(&threadInput, NULL, inputThread, NULL) != 0) {
        perror("Error threadInput creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown inputThread
void input_shutdown()
{
    pthread_cancel(threadInput);
    pthread_join(threadInput, NULL);
}