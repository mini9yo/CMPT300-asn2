#ifndef _SEND_H
#define _SEND_H

// Send thread implementation
void* sendThread(void* threadArgs);

// Send signaller
void send_signal();

// Initialize sendThread
<<<<<<< HEAD
void send_init(List* list, int port, int remotePort);
=======
void send_init(List* messageListSend, int socketDescriptor, char * remoteMachine, int remotePort);
>>>>>>> 2be837e128123a1064baf993d96530fd4b241258

// Shutdown sendThread
void send_waitForShutdown();

#endif