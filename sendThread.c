#include "sendThread.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"


static pthread_t threadSend;
static List* sendList;
static pthread_cond_t s_sendCond = PTHREAD_COND_INITIALIZER;
static int sendShutdown = 0;
static int socketDescriptor = -1;
static int PORT = -1;

typedef struct params {
    List* list;
    int socket;
    char* machine;
    int port;
} threadParams;

// Send thread implementation
void* sendThread(void* threadArgs)
{
    // Initialize argument variables
    threadParams* args = (threadParams*)threadArgs;
    sendList = args->list;
    PORT = args->port;

    // socket
    if (PORT == -1) {
        perror("Error PORT initialization");
        exit(EXIT_FAILURE);
    }
    while(1) {
        pthread_mutex_lock(&s_sendMutex);
        while (List_count(sendList) == 0 && !sendShutdown) {
            pthread_cond_wait(&s_sendCond, &s_sendMutex);
        }
        if (sendShutdown) {
            pthread_mutex_unlock(&s_sendMutex);
            break;
        }
        // retrieve message from list
        char* message = List_trim(sendList);
        pthread_mutex_unlock(&s_sendMutex);
        // send message
        struct sockaddr_in sin;
        memset(&sin, 0, sizeof(sin));
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = htonl(INADDR_ANY);
        sin.sin_port = htons(PORT);
        if (sendto(socketDescriptor, message, strlen(message), 0, (struct sockaddr*) &sin, sizeof(sin)) < 0) {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }
        free(message);
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
    threadParams threadArgs = {messageListSend, socketDescriptor, remoteMachine, remotePort};
    if (pthread_create(&threadSend, NULL, sendThread, (void*) &threadArgs) != 0) {
        perror("Error threadSend creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown sendThread
void send_shutdown()
{
    pthread_join(threadSend, NULL);
}
