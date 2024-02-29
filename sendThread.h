#ifndef _SEND_H
#define _SEND_H

#include "list.h"

// Send thread implementation
void* sendThread(void* threadArgs);

// Initialize sendThread
void send_init(List* messageListSend, int socketDescriptor, char * remoteMachine, int remotePort);

// Send signaller
void send_signal();

// Cancel sendThread
void cancelSend();

// Shutdown sendThread
void send_shutdown();

#endif