//include header file to support queuing functions
#include "q.h"

extern TCB_t * execution_q;

void create_thread(void (*function)(void *))
{

    void * stackp;
    TCB_t *tcb;

    stackp = malloc(8192);              //allocate a stack (via malloc) of a certain size (choose 8192)
    tcb = (TCB_t*)malloc(sizeof(tcb));  //allocate a TCB (via malloc)

    init_TCB(tcb,function,stackp,8192); //call init_TCB with appropriate arguments (stack size 8192)
    AddQueue(&execution_q,tcb);         //call AddQueue to add this TCB into the execution queue which is a global header pointer
}

void run()
{
    ucontext_t parent;                                     // get a place to store the main context
    getcontext(&parent);

    // use swapcontext() to start executing first thread element in execution queue
    swapcontext(&parent,&execution_q->context);
}

void yield()
{
    //rotate the execution queue
    RotateQueue(&execution_q);

    //swap the context, from previous thread element to the thread pointed to by execution queue
    swapcontext(&execution_q->prev->context,&execution_q->context);
}
