#ifndef _PRINT_H
#define _PRINT_H

// Print thread implementation
void* printThread();

// Initialize printThread
void print_init();

// Shutdown printThread
void print_waitForShutdown();

void print_signal();

#endif