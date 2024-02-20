#ifndef _PRINT_H
#define _PRINT_H

// Print thread implementation
void* printThread();

// Print signaller
void print_signal();

// Initialize printThread
void print_init();

// Shutdown printThread
void print_waitForShutdown();

#endif