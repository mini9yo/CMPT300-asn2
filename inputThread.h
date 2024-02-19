#ifndef _INPUT_H
#define _INPUT_H

// Input thread implementation
void* inputThread();

// Initialize inputThread
void input_init();

// Shutdown inputThread
void input_waitForShutdown();

#endif