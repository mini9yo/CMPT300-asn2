#ifndef _SEND_H
#define _SEND_H

#include "list.h"
#include <pthread.h>

static pthread_mutex_t s_sendMutex = PTHREAD_MUTEX_INITIALIZER;

// Send thread implementation
void* sendThread(void* threadArgs);

// Initialize sendThread
void send_init(List* messageListSend, int socketDescriptor, char * remoteMachine, int remotePort);

void send_signal();

// Shutdown sendThread
void send_shutdown();

#endif
