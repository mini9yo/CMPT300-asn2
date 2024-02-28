#include "receiveThread.h"
#include "printThread.h"
#include "list.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include "listOps.h"

#define BUFFER_MAX_LEN 256

// Initialize variables
static pthread_t threadReceive;

typedef struct params {
    List* list;
    int socket;
} threadParams;

// Receive thread implementation
void* receiveThread(void* threadArgs)
{
    // Initialize argument variables
    threadParams* args = (threadParams*)threadArgs;
    List* listRx = args->list;
    int socketDescriptor = args->socket;

    // Initialize (other) variables
    char msgRx[BUFFER_MAX_LEN];
    char* s_msgRx_allocated;
    int msgRx_len;
    
    while (1) {
        // Retrieve message from socket
        struct sockaddr_in sinRemote;
        unsigned int sin_len = sizeof(sinRemote);
        memset(&msgRx, 0, BUFFER_MAX_LEN);
        msgRx_len = recvfrom(socketDescriptor,
            msgRx, BUFFER_MAX_LEN, 0,
            (struct sockaddr *) &sinRemote, &sin_len);

        // Copy message into allocated memory
        s_msgRx_allocated = strdup(msgRx);
        if (s_msgRx_allocated == NULL) {
            perror("Error memory allocation for outbound message list item");
            exit(EXIT_FAILURE);
        }

        // Add message item to list
        if (listAdd(listRx, s_msgRx_allocated) == -1) {
            perror("Error adding item to inbound message list");
            exit(EXIT_FAILURE);
        }

        // Signal print thread
        print_signal();

        // Check for exit code ('!')
        if ((msgRx_len == 1) && (msgRx[0] == '!')) {
            printf("Exit code detected. S-talk session terminated.");
            break;
        }
    }

    return NULL;
}

// Initialize receiveThread
void receive_init(List* listRx, int socketDescriptor)
{
    threadParams threadArgs = {listRx, socketDescriptor};
    if (pthread_create(&threadReceive, NULL, receiveThread, (void*) &threadArgs) != 0) {
        perror("Error threadReceive creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown receiveThread
void receive_shutdown()
{
    pthread_cancel(threadReceive);
    pthread_join(threadReceive, NULL);

    // Free allocated memory
    if (s_msgRx_allocated) {
        free(s_msgRx_allocated);
        s_msgRx_allocated = NULL;
    }
}