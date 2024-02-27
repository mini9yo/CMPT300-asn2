#ifndef _RECEIVE_H
#define _RECEIVE_H

#include "list.h"

// Receive thread implementation
void* receiveThread(void* threadArgs);

// Initialize receiveThread
void receive_init(List* listRx, int socketDescriptor);

// Shutdown receiveThread
void receive_shutdown();

#endif