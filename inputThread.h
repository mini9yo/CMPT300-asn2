#ifndef _INPUT_H
#define _INPUT_H

#include "list.h"

// Input thread implementation
void* inputThread();

// Initialize inputThread
void input_init(List* list);

// Cancel inputThread
void cancelInput();

// Shutdown inputThread
void input_shutdown();

#endif