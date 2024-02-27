#include "sendThread.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "list.h"
#include "listOps.h"

static pthread_t threadSend;
static List* sendList;
static pthread_mutex_t s_sendMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_sendCond = PTHREAD_COND_INITIALIZER;
static int sendShutdown = 0;
static int socketDescriptor = -1;
static int PORT = -1;

<<<<<<< HEAD
=======
typedef struct params {
    List* list;
    int socket;
    char* machine;
    int port;
} threadParams;

>>>>>>> 2be837e128123a1064baf993d96530fd4b241258
// Send thread implementation
void* sendThread(void* threadArgs)
{
<<<<<<< HEAD
=======
    // Initialize argument variables
    threadParams* args = (threadParams*)threadArgs;
    sendList = args->list;
    PORT = args->port;

    // socket
    if (PORT == -1) {
        perror("Error PORT initialization");
        exit(EXIT_FAILURE);
    }

>>>>>>> 2be837e128123a1064baf993d96530fd4b241258
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
        char* message = listRemove(sendList);
<<<<<<< HEAD
        pthread_mutex_unlock(&s_sendMutex);
=======
        pthread_mutex_unlock(&sendMutex);
>>>>>>> 2be837e128123a1064baf993d96530fd4b241258

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
<<<<<<< HEAD
void send_init(List* list, int port, int remotePort)
=======
void send_init(List* list, int socketDescriptor, char* machine, int port)
>>>>>>> 2be837e128123a1064baf993d96530fd4b241258
{
    threadParams threadArgs = {list, socketDescriptor, machine, port};
    if (pthread_create(&threadSend, NULL, sendThread, (void*) &threadArgs) != 0) {
        perror("Error threadSend creation");
        exit(EXIT_FAILURE);
    }
}

// Shutdown sendThread
void send_waitForShutdown()
{
    pthread_join(threadSend, NULL);
}