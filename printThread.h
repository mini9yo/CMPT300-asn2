#ifndef _PRINT_H
#define _PRINT_H

// Print thread implementation
void* printThread(void* listRx);

// Print signaller
void print_signal();

// Initialize printThread
void print_init(List* listRx);

// Shutdown printThread
void print_shutdown();

void print_signal();

#endif