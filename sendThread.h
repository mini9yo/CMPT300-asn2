#ifndef _SEND_H
#define _SEND_H

// Send thread implementation
void* sendThread();

// Initialize sendThread
void send_init();

// Shutdown sendThread
void send_waitForShutdown();

#endif