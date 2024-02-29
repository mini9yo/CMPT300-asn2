#include "sendThread.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"

typedef struct params {
    List* list;
    int socket;
    char* machine;
    int port;
} threadParams;

static pthread_t threadSend;
static pthread_mutex_t s_sendMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_sendCond = PTHREAD_COND_INITIALIZER;
static int sendShutdown = 0;

static threadParams* threadArgs;

// Send thread implementation
void* sendThread(void* threadArgs)
{
    // Initialize argument variables
    threadParams* params = (threadParams*)threadArgs;
    int socketDescriptor = params->socket;
    int PORT = params->port;
    char* machine = params->machine;
    List* sendList = params->list;

    // // test the parameters: PASSED
    //printf("socketDescriptor: %d\n", socketDescriptor);
    // printf("PORT: %d\n", PORT);
    // printf("machine: %s\n", machine);
    
    if (socketDescriptor < 0 || PORT < 0 || machine == NULL) {
        perror("Error: Invalid parameters in sendThread");
        exit(EXIT_FAILURE);
    }

    while(1) {
        pthread_mutex_lock(&s_sendMutex);
        {
            pthread_cond_wait(&s_sendCond, &s_sendMutex);
        }
        pthread_mutex_unlock(&s_sendMutex);

        // send message
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));

        char* message = NULL;
        pthread_mutex_lock(&s_sendMutex);
        {
            // retrieve message from list
            message = List_trim(sendList);
        }
        pthread_mutex_unlock(&s_sendMutex);

        struct hostent *remote = gethostbyname(machine);
        if (remote == NULL) {
            perror("Error getting host by name");
            exit(EXIT_FAILURE);
        } 

        sin.sin_family = AF_INET;
        memcpy(&sin.sin_addr, remote->h_addr_list[0], sizeof(remote->h_length));
        sin.sin_port = htons(PORT);
        
        if (message != NULL) {
            if (sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
                perror("Error sending message");
                exit(EXIT_FAILURE);
            } 
        }

        if(strcmp("!\n", message) == 0) {
            sendShutdown = 1;
        }

    }
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
void send_init(List* messageListSend, int socketDescriptor, char * remoteMachine, int remotePort)
{
    threadArgs = (threadParams*) malloc(sizeof(threadParams));
    if (threadArgs == NULL) {
        perror("Error memory allocation for sendThread arguments");
        exit(EXIT_FAILURE);
    }
    threadArgs->list = messageListSend;
    threadArgs->socket = socketDescriptor;
    threadArgs->machine = remoteMachine;
    threadArgs->port = remotePort;

    if (pthread_create(&threadSend, NULL, sendThread, (void*) threadArgs) != 0) {
        perror("Error threadSend creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown sendThread
void send_shutdown()
{
    pthread_join(threadSend, NULL);
}