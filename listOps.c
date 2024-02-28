#include <pthread.h>
#include "list.h"
#include "listOps.h"

static pthread_mutex_t s_listMutex = PTHREAD_MUTEX_INITIALIZER;

// Add item to list
int listAdd(List* list, char* msg)
{
    int result;
    pthread_mutex_lock(&s_listMutex);
    {
        result = List_prepend(list, msg);
    }
    pthread_mutex_unlock(&s_listMutex);

    return result;
}

// Remove item from list
char* listRemove(List* list)
{
    char* result;
    pthread_mutex_lock(&s_listMutex);
    {
        result = List_trim(list);
    }
    pthread_mutex_unlock(&s_listMutex);

    return result;
}