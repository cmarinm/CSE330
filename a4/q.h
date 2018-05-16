//include header file for TCB
#include "tcb.h"
#include <stdlib.h>
//creates an empty queue, pointed to by the variable head.
void InitQueue(TCB_t* header)
{
    header = NULL;
}

//adds an element at the end of the queue
void AddQueue(TCB_t **header, TCB_t * item)
{
    // you need to deal with prev and next
    // pointers of the item and given header
    if((*header) == NULL){ //if the stack is empty, then the item will become the header
        (*header) = item;
        (*header)->next = NULL;
        (*header)->prev = NULL;
    }
    else{
        if((*header)->next == NULL){ //if there is only 1 item (the head)
            (*header)->next = item;
            item->next = (*header);
            item->prev = (*header);
            (*header)->prev = item;
        } else{ //the list is not empty, neither does it have only 1 item, so it is already in a circular doubly linked structure
            item->prev = (*header)->prev;
            item->next = (*header);
            (*header)->prev->next = item;
            (*header)->prev = item;
        }
    }

}

// Moves the header pointer to the next element in the queue.
void RotateQueue(TCB_t **header)
{
    if((*header)->next != NULL)
        (*header) = (*header)->next;
}
