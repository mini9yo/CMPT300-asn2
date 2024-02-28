#ifndef _LIST_OPS_H
#define _LIST_OPS_H

#include "list.h"

// Add item to list
int listAdd(List* list, char* msg);

// Remove item from list
void* listRemove(List* list);

#endif