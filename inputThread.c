#include "inputThread.h"
#include "sendThread.h"
#include "list.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "canceller.h"


#define BUFFER_MAX_LEN 256

static pthread_t threadInput;
static List* inputList;
static pthread_mutex_t inputMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t inputCond = PTHREAD_COND_INITIALIZER;
static char* inputBuffer = NULL;

// Input thread implementation
void* inputThread()
{
    while (1) {
        inputBuffer = (char*) malloc(BUFFER_MAX_LEN * sizeof(char));
        fgets(inputBuffer, BUFFER_MAX_LEN, stdin);

        if (List_prepend(inputList, strdup(inputBuffer)) != 0) {
            perror("List add failed\n");
            exit(EXIT_FAILURE);
        }

        if(List_count(inputList) == 1) {
            send_signal();
        }  

        if (strcmp("!\n", inputBuffer) == 0) {
            free(inputBuffer);
            receiveCancel();
            break;
        }
        free(inputBuffer);
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

// Cancel inputThread
void cancelInput()
{
    pthread_cancel(threadInput);
}

// Shutdown inputThread
void input_shutdown()
{
    pthread_join(threadInput, NULL);
}