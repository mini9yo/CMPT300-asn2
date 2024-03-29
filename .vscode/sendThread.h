#ifndef _SEND_H
#define _SEND_H

#include "list.h"

// Send thread implementation
void* sendThread(void* threadArgs);

// Initialize sendThread
void send_init(List* messageListSend, int socketDescriptor, char * remoteMachine, int remotePort);

void send_signal();

void cancelSend();

// Shutdown sendThread
void send_shutdown();

#endif