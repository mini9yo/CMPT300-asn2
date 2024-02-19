#ifndef _RECEIVE_H
#define _RECEIVE_H

// Receive thread implementation
void* receiveThread();

// Initialize receiveThread
void receive_init();

// Shutdown receiveThread
void receive_waitForShutdown();

#endif