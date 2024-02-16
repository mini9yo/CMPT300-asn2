#include "list.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

// Global variables
static List listArray[LIST_MAX_NUM_HEADS];
static Node nodeArray[LIST_MAX_NUM_NODES];
static List* freeHead = NULL;
static Node* freeNode = NULL;
static int freeHead_num = 0;
static int freeNode_num = 0;

// Private function that pushes free Nodes
static void freeNode_push(Node* newNode)
{
    assert(newNode != NULL);
    assert(freeNode_num <= LIST_MAX_NUM_NODES);

    // freeNode array empty
    if (freeNode_num == 0) { 
        newNode->nextFree = NULL;
    } else {
        newNode->nextFree = freeNode;
    }

    freeNode = newNode;
    freeNode_num++;

    return;
}

// Private function that pops free Nodes
static Node* freeNode_pop()
{
    assert(freeNode != NULL);
    assert(freeNode_num > 0);

    Node* removeNode = freeNode;

    // Popping the last free Node
    if (freeNode_num == 1) { 
        freeNode = NULL;
    } else {
        freeNode = freeNode->nextFree;
    }

    removeNode->nextFree = NULL;
    freeNode_num--;

    return removeNode;
}

// Private function that pushes free Heads
static void freeHead_push(List* newHead) 
{
    assert(newHead != NULL);
    assert(freeHead_num <= LIST_MAX_NUM_HEADS);

    // freeNode array empty
    if (freeHead_num == 0) { 
        newHead->nextFree = NULL;
    } else {
        newHead->nextFree = freeHead;
    }
    
    freeHead = newHead;
    freeHead_num++;
    
    return;
}

// Private function that pops free Heads
static List* freeHead_pop()
{
    assert(freeHead != NULL);
    assert(freeHead_num > 0);

    List* removeHead = freeHead;
    
    // Popping the last free Node
    if (freeHead_num == 1) { 
        freeHead = NULL;
    } else {
        freeHead = freeHead->nextFree;  
    }

    removeHead->nextFree = NULL;
    freeHead_num--;

    return removeHead;
}

// Makes a new, empty list, and returns its reference on success. 
// Returns a NULL pointer on failure.
List* List_create()
{
    static int firstCall = 1;

    if (firstCall) {
        
        // Create list of free heads
        freeHead_num = 0;
        freeHead = &listArray[LIST_MAX_NUM_HEADS-1];
        for (int i = LIST_MAX_NUM_HEADS-1; i >= 0; i--) {
            freeHead_push(&listArray[i]);
        }

        // Create list of free nodes
        freeNode_num = 0;
        freeNode = &nodeArray[LIST_MAX_NUM_NODES-1];
        for (int i = LIST_MAX_NUM_NODES-1; i >= 0; i--) {
            freeNode_push(&nodeArray[i]);
        }
        firstCall = 0;
    } 

    // No free list heads
    if (freeHead_num < 1) { 
        return NULL;
    }

    // Initialize new list
    List* newList = freeHead_pop();
    newList->current = NULL;
    newList->first = NULL;
    newList->last = NULL;
    newList->numItems = 0;
    newList->state = LIST_OOB_START;

    return newList;
}

// Returns the number of items in pList.
int List_count(List* pList)
{
    assert(pList != NULL);

    return pList->numItems;
}

// Returns a pointer to the first item in pList and makes the first item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_first(List* pList)
{
    assert(pList != NULL);
    assert(pList->numItems > -1);

    // List is empty
    if (pList->numItems == 0) { 
        pList->current = NULL;
        return NULL;
    }

    pList->current = pList->first;

    return pList->first->val;
}

// Returns a pointer to the last item in pList and makes the last item the current item.
// Returns NULL and sets current item to NULL if list is empty.
void* List_last(List* pList)
{
    assert(pList != NULL);
    assert(pList->numItems > -1);

    // List is empty
    if (pList->numItems == 0) { 
        pList->current = NULL;
        return NULL;
    }

    pList->current = pList->last;

    return pList->last->val;
}

// Advances pList's current item by one, and returns a pointer to the new current item.
// If this operation advances the current item beyond the end of the pList, a NULL pointer 
// is returned and the current item is set to be beyond end of pList.
void* List_next(List* pList)
{
    assert(pList != NULL);

    // Current pointer out of bounds
    if (pList->current == NULL) { 
        if ((pList->state == LIST_OOB_START) && (pList->numItems >0)) {
            pList->current = pList->first;
            return pList->current->val;
        } else {
            return NULL;
        }
    }

    // Item advances to beyond end of pList
    if (pList->current == pList->last) { 
        pList->state = LIST_OOB_END;
        pList->current = NULL;
        return NULL;
    }

    // Advance current item by one
    pList->current = pList->current->next;

    return pList->current->val;
}

// Backs up pList's current item by one, and returns a pointer to the new current item. 
// If this operation backs up the current item beyond the start of the pList, a NULL pointer 
// is returned and the current item is set to be before the start of pList.
void* List_prev(List* pList) 
{
    assert(pList != NULL);
    
    // Current pointer out of bounds
    if (pList->current == NULL) { 
        if ((pList->state == LIST_OOB_END) && (pList->numItems >0)) {
            pList->current = pList->last;
            return pList->current->val;
        } else {
            return NULL;
        }
    }

    // Item backs up beyond start of pList
    if (pList->current == pList->first) { 
        pList->state = LIST_OOB_START;
        pList->current = NULL;
        return NULL;
    }

    // Back up current item by one
    pList->current = pList->current->prev;

    return pList->current->val;
}

// Returns a pointer to the current item in pList.
void* List_curr(List* pList) 
{
    assert(pList != NULL);

    if (pList->current == NULL) {
        return NULL;
    }

    return pList->current->val;
}

// Adds the new item to pList directly after the current item, and makes item the current item. 
// If the current pointer is before the start of the pList, the item is added at the start. If 
// the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_after(List* pList, void* pItem)
{
    assert(pList != NULL);
    assert(pItem != NULL);

    if (freeNode_num < 1) { // No space for adding item
        return -1;
    }

    Node* newNode = freeNode_pop();
    newNode->val = pItem;
    
    // List is initially empty
    if (pList->numItems == 0) {
        newNode->next = NULL;
        newNode->prev = NULL;
        pList->first = newNode;
        pList->last = newNode;
    // Current pointer before start of nonempty list
    } else if ((pList->current == NULL) && (pList->state == LIST_OOB_START)) { 
        newNode->next = pList->first;
        newNode->prev = NULL;
        pList->first->prev = newNode;
        pList->first = newNode;
    // Current pointer after end of nonempty list or at end of list
    } else if (((pList->current == NULL) && (pList->state == LIST_OOB_END)) || (pList->current == pList->last)) {
        newNode->next = NULL;
        newNode->prev = pList->last;
        pList->last->next = newNode;
        pList->last = newNode;
    // Current pointer within bounds
    } else { 
        newNode->prev = pList->current;
        newNode->next = pList->current->next;
        pList->current->next->prev = newNode;
        pList->current->next = newNode;
    }

    pList->current = newNode;
    pList->numItems++;

    return 0;
}

// Adds item to pList directly before the current item, and makes the new item the current one. 
// If the current pointer is before the start of the pList, the item is added at the start. 
// If the current pointer is beyond the end of the pList, the item is added at the end. 
// Returns 0 on success, -1 on failure.
int List_insert_before(List* pList, void* pItem)
{
    assert(pList != NULL);
    assert(pItem != NULL);

    if (freeNode_num < 1) { // No space for adding item
        return -1;
    }

    Node* newNode = freeNode_pop();
    newNode->val = pItem;

    // List is initially empty
    if (pList->numItems == 0) {
        newNode->next = NULL;
        newNode->prev = NULL;
        pList->first = newNode;
        pList->last = newNode;
    // Current pointer before start of nonempty list or at start of list
    } else if (((pList->current == NULL) && (pList->state == LIST_OOB_START)) || (pList->current == pList->first)) { // current pointer out of bounds
        newNode->next = pList->first;
        newNode->prev = NULL;
        pList->first->prev = newNode;
        pList->first = newNode;
    // Current pointer after start of nonempty list or at the end of list
    } else if ((pList->current == NULL) && (pList->state == LIST_OOB_END)) {
        newNode->next = NULL;
        newNode->prev = pList->last;
        pList->last->next = newNode;
        pList->last = newNode;
    // Current pointer within bounds
    } else {
        newNode->prev = pList->current->prev;
        newNode->next = pList->current;
        pList->current->prev->next = newNode;
        pList->current->prev = newNode;
    }

    pList->current = newNode;
    pList->numItems++;

    return 0;
}

// Adds item to the end of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_append(List* pList, void* pItem)
{
    assert(pList != NULL);
    assert(pItem != NULL);

    if (freeNode_num < 1) { // No space for adding item
        return -1;
    }
    
    Node* newNode = freeNode_pop();
    newNode->val = pItem;

    newNode->next = NULL;
    newNode->prev = pList->last;

    if (pList->numItems > 0) {
        pList->last->next = newNode;
    }

    pList->last = newNode;
    pList->current = newNode;
    pList->numItems++;

    if (pList->numItems == 1) {
        pList->first = pList->current;
    }

    return 0;
}

// Adds item to the front of pList, and makes the new item the current one. 
// Returns 0 on success, -1 on failure.
int List_prepend(List* pList, void* pItem)
{
    assert(pList != NULL);
    assert(pItem != NULL);

    if (freeNode_num < 1) { // No space for adding item
        return -1;
    }
    
    Node* newNode = freeNode_pop();
    newNode->val = pItem;

    newNode->next = pList->first;
    newNode->prev = NULL;

    if (pList->numItems > 0) {
        pList->first->prev = newNode;
    }

    pList->first = newNode;
    pList->current = newNode;
    pList->numItems++;

    if (pList->numItems == 1) {
        pList->last = pList->current;
    }

    return 0;
}

// Return current item and take it out of pList. Make the next item the current one.
// If the current pointer is before the start of the pList, or beyond the end of the pList,
// then do not change the pList and return NULL.
void* List_remove(List* pList)
{
    assert(pList != NULL);

    // Current pointer out of bounds
    if (pList->current == NULL) { 
        return NULL;
    }
    
    void* res = pList->current->val;
    // One node remaining
    if (pList->numItems == 1) {
        pList->first = NULL;
        pList->last = NULL;
    // Current points to first node
    } else if (pList->current == pList->first) { 
        pList->first->next->prev = NULL;
        pList->first = pList->first->next;
    // Current points to last node
    } else if (pList->current == pList->last) { 
        pList->last->prev->next = NULL;
        pList->last = pList->last->prev;
    // Current points to middle node
    } else { 
        pList->current->prev->next = pList->current->next;
        pList->current->next->prev = pList->current->prev;
    }
    
    freeNode_push(pList->current);
    pList->current = pList->current->next;
    // Updated current points beyond end of list
    if (pList->current == NULL) { 
        pList->state = LIST_OOB_END;
    }

    pList->numItems--;

    return res;
}

// Return last item and take it out of pList. Make the new last item the current one.
// Return NULL if pList is initially empty.
void* List_trim(List* pList)
{
    assert(pList != NULL);

    void* lastItem;

    // Return NULL if pList is initially empty.
    if (pList->numItems < 1) {
        lastItem = NULL;
    } else {
        lastItem = pList->last->val;

        // Make new last item the current one
        pList->current = pList->last->prev;

        // Remove last item from pList
        freeNode_push(pList->last); // Push last item into the free pool

        // Update pointers
        if (pList->current == NULL) {
            pList->state = LIST_OOB_START;
            pList->first = NULL;
            pList->last = NULL;
        } else {
            pList->last->prev = NULL;
            pList->last = pList->current;
            pList->last->next = NULL;
        }
        
        pList->numItems--;
    }

    return lastItem;
}

// Adds pList2 to the end of pList1. The current pointer is set to the current pointer of pList1. 
// pList2 no longer exists after the operation; its head is available
// for future operations.
void List_concat(List* pList1, List* pList2)
{
    assert(pList1 != NULL);
    assert(pList2 != NULL);
    assert(pList1 != pList2);

    if (pList1->numItems == 0) {
        pList1->first = pList2->first;
        pList1->last = pList2->last;
    } else if (pList2->numItems != 0) {
        // Add pList2 to the end of pList1
        pList1->last->next = pList2->first;
        pList2->first->prev = pList1->last;
        pList1->last = pList2->last;
    }

    pList1->numItems += pList2->numItems;

    freeHead_push(pList2);

    return;
}

// Delete pList. pItemFreeFn is a pointer to a routine that frees an item. 
// It should be invoked (within List_free) as: (*pItemFreeFn)(itemToBeFreedFromNode);
// pList and all its nodes no longer exists after the operation; its head and nodes are 
// available for future operations.
typedef void (*FREE_FN)(void* pItem);
void List_free(List* pList, FREE_FN pItemFreeFn)
{
    assert(pList != NULL);

    void* pItemFree;
    Node* temp = pList->first;
    for (int i = 0; i < pList->numItems; i++) {
        pItemFree = temp->val;
        (*pItemFreeFn)(pItemFree);

        // Add node to free pool
        freeNode_push(temp);

        temp = temp->next;
    }

    freeHead_push(pList);

    return;
}

// Search pList, starting at the current item, until the end is reached or a match is found. 
// In this context, a match is determined by the comparator parameter. This parameter is a
// pointer to a routine that takes as its first argument an item pointer, and as its second 
// argument pComparisonArg. Comparator returns 0 if the item and comparisonArg don't match, 
// or 1 if they do. Exactly what constitutes a match is up to the implementor of comparator. 
// 
// If a match is found, the current pointer is left at the matched item and the pointer to 
// that item is returned. If no match is found, the current pointer is left beyond the end of 
// the list and a NULL pointer is returned.
// 
// If the current pointer is before the start of the pList, then start searching from
// the first node in the list (if any).
typedef bool (*COMPARATOR_FN)(void* pItem, void* pComparisonArg);
void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparisonArg)
{
    assert(pList != NULL);
    assert(pComparator != NULL);
    assert(pComparisonArg != NULL);

    if (pList->current == NULL) {
        if (pList->state == LIST_OOB_START) {
            pList->current = pList->first;
        } 
        if (pList->state == LIST_OOB_END) {
            return NULL;
        }
    }

    void* res = NULL;
    int match = 0;
    while ((pList->current != NULL) && (match == 0)) {
        match = (*pComparator)(pList->current->val, pComparisonArg);
        if (match == 1) {
            res = pList->current->val;
            return res;
        }
        pList->current = pList->current->next;
    }

    return res;
}
