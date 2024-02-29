#ifndef _PRINT_H
#define _PRINT_H

#include "list.h"

// Print thread implementation
void* printThread(void* threadArg);

// Print signaller
void print_signal();

// Initialize printThread
void print_init(List* listRx);

// Cancel printThread
void cancelPrint();

// Shutdown printThread
void print_shutdown();

#endif