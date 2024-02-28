#include "inputThread.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "list.h"
#include "listOps.h"

#define BUFFER_MAX_LEN 256

static pthread_t threadInput;
static List* inputList;
static int inputShutdown = 0;

// Input thread implementation
void* inputThread()
{
    // Initialize variables
    char inputBuffer[BUFFER_MAX_LEN];
    char* msg;
    int msgSize;

    while (1) {
        // Reset buffer
        memset(&inputBuffer, 0, BUFFER_MAX_LEN);
        
        // Read input into buffer
        fgets(inputBuffer, BUFFER_MAX_LEN, stdin);
        
        // Replace newline char with null terminator if present
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

        // Copy message into allocated memory
        msg = strdup(inputBuffer);
        if (msg == NULL) {
            perror("Error memory allocation for outbound message list item");
            exit(EXIT_FAILURE);
        }

        // Add message item to list
        if (listAdd(inputList, msg) != 0) {
            perror("List add failed\n");
            exit(EXIT_FAILURE);
        }

        // Signal print thread
        send_signal();

        // Check for exit code ('!')
        if (strcmp("!\0", msg) == 0) {
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
