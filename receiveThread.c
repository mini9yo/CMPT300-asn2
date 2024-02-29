#include "receiveThread.h"
#include "printThread.h"
#include "list.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include "canceller.h"

#define BUFFER_MAX_LEN 256

// Initialize variables
static pthread_t threadReceive;
static char* s_msgRx_allocated = NULL;
static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct params {
    List* list;
    int socket;
} threadParams;

static threadParams* threadArgs;

// Receive thread implementation
void* receiveThread(void* threadArgs)
{
    // Initialize argument variables
    threadParams* params = (threadParams*)threadArgs;
    int socketDescriptor = params->socket;
    List* listRx = params->list;

    // // test the parameters: PASSED
    // printf("socketDescriptor: %d\n", socketDescriptor);
    
    while (1) {
        // Retrieve message from socket
        struct sockaddr_in sinRemote;
        memset(&sinRemote, 0, sizeof(sinRemote));
        unsigned int sin_len = sizeof(sinRemote);
 
        // Create message item and copy over message
        // Allocate memory size of message length + 1 for the null terminator
        s_msgRx_allocated = (char*) malloc((BUFFER_MAX_LEN) * sizeof(char));
        if (s_msgRx_allocated == NULL) {
            perror("Error memory allocation for inbound message list item");
            exit(EXIT_FAILURE);
        }

        if (recvfrom(socketDescriptor,
            s_msgRx_allocated, BUFFER_MAX_LEN, 0,
            (struct sockaddr *) &sinRemote, &sin_len) < 0){
                perror("Error receiving message");
                exit(EXIT_FAILURE);
        }

        // Remove newline character from the message
        size_t length = strlen(s_msgRx_allocated);
        if (length > 0 && s_msgRx_allocated[length - 1] == '\n') {
            s_msgRx_allocated[length -1] = '\0';
        }

        pthread_mutex_lock(&printMutex);
        {
            if (List_prepend(listRx, strdup(s_msgRx_allocated)) == -1) {
                perror("Error adding item to inbound message list");
                exit(EXIT_FAILURE);
            }
        }
        pthread_mutex_unlock(&printMutex);

        // Signal print thread
        pthread_mutex_lock(&printMutex);
        {
            if (List_count(listRx) == 1) {
                print_signal();
            }
        }
        pthread_mutex_unlock(&printMutex);

        // Check for exit code ('!')
        if ((strcmp(s_msgRx_allocated, "!\n") == 0) || (strcmp(s_msgRx_allocated, "!\0")) == 0){
            sendCancel();
            free(s_msgRx_allocated);
            break;
        }
        free(s_msgRx_allocated);
    }
    return NULL;
}

// Initialize receiveThread
void receive_init(List* listRx, int socketDescriptor)
{
    threadArgs = (threadParams*) malloc(sizeof(threadParams));
    if (threadArgs == NULL) {
        perror("Error memory allocation for sendThread arguments");
        exit(EXIT_FAILURE);
    }
    threadArgs->list = listRx;
    threadArgs->socket = socketDescriptor;

    if (pthread_create(&threadReceive, NULL, receiveThread, (void*) threadArgs) != 0) {
        perror("Error threadReceive creation");
        exit(EXIT_FAILURE);
    }
}

// Cancel receiveThread
void cancelReceive()
{
    pthread_cancel(threadReceive);
}

// Shutdown receiveThread
void receive_shutdown()
{
    free(threadArgs);
    pthread_join(threadReceive, NULL);

    // Free allocated memory
    // if (s_msgRx_allocated) {
    //     free(s_msgRx_allocated);
    //     s_msgRx_allocated = NULL;
    // }
}