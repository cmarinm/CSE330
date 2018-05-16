#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct block{
    bool hole;
    int size_kb;
    int pid;
    int s_address;
    struct block *next;
}block;
typedef struct process{
    int pid;
    int p_size;
    struct process *next;
}process;

struct block *allocation_queue;
struct process *wait_queue;

void rem_wait(int processID);

void add_wait(int processID, int memSize);

void allocateMem(int processID, int memSize){
    bool allocated = false;
    struct block *temp = allocation_queue;
    if(processID == 0){
        allocated = true;
        printf("Block of empty memory allocated with given size\n");
        struct block *newNode = (block*)malloc(sizeof(block));
        newNode->hole = true;
        newNode->pid = 0;
        newNode->s_address = 0;
        newNode->size_kb = memSize;
        newNode->next = NULL;
        allocation_queue = newNode;
    } else{
        //if the first item is a hole and big enough
        if(temp->hole && temp->size_kb >= memSize){
            struct block *newNode = (block*)malloc(sizeof(block));
            newNode->hole = false;
            newNode->pid = processID;
            newNode->size_kb = memSize;
            newNode->s_address = 0;

            temp->size_kb = temp->size_kb - memSize;
            temp->s_address = memSize;

            newNode->next = temp;
            allocation_queue = newNode;
            allocated = true;
        } else{
            while(temp->next != NULL && !allocated){
                if(temp->next->hole && temp->next->size_kb >= memSize){
                    allocated = true;
                    struct block *newNode = (block*)malloc(sizeof(block));
                    newNode->hole = false;
                    newNode->pid = processID;
                    newNode->size_kb = memSize;
                    newNode->s_address = temp->next->s_address;

                    temp->next->s_address = temp->next->s_address + memSize;
                    temp->next->size_kb = temp->next->size_kb - memSize;
                    newNode->next = temp->next;
                    temp->next = newNode;

                }
                temp = temp->next;
            }
        }
    }

    if(!allocated){
        printf("Not enough space for process, adding process to wait queue...\n");
        add_wait(processID,memSize);
    }

}

void deleteMem(int processID){
    bool found = false;
    struct block *temp = allocation_queue;
    while(temp != NULL && !found){
        if(temp->pid == processID){
            found = true;
            temp->pid = 0;
            temp->hole = true;
        }
         temp = temp->next;
    }
    if(!found)
        printf("Process not found in Allocation Queue\n");
    temp = allocation_queue;
    struct block *del;
    //now iterate through list again to see if we find two adjacent holes and if so unite them
    while(temp != NULL && temp->next != NULL){
        if(temp->hole && temp->next->hole){
            temp->size_kb = temp->size_kb + temp->next->size_kb;
            del = temp->next;
            temp->next = del->next;
            free(del);
        }
        temp = temp->next;
    }
    // we have to go through list twice to get all blocks.
    temp = allocation_queue;
    while(temp != NULL && temp->next != NULL){
        if(temp->hole && temp->next->hole){
            temp->size_kb = temp->size_kb + temp->next->size_kb;
            del = temp->next;
            temp->next = del->next;
            free(del);
        }
        temp = temp->next;
    }

    //now iterate through wait list to see if any process can be allocated now.
    struct process *tempw;
    tempw = wait_queue;
    int size;
    while(tempw != NULL){
        size = tempw->p_size;
        temp = allocation_queue;
        bool allocated = false;

        while(temp != NULL && !allocated){
            if(temp->hole && (temp->size_kb >= size)){
                allocated = true;
                allocateMem(tempw->pid,tempw->p_size);
                rem_wait(tempw->pid);
            }
            temp = temp->next;
        }

        tempw = tempw->next;
    }



}

void add_wait(int processID, int memSize){
    process *newP = (process*)malloc(sizeof(struct process));
    newP->pid = processID;
    newP->p_size = memSize;
    if(wait_queue == NULL){
        newP->next = NULL;
        wait_queue = newP;
    }
    else{
        newP->next = wait_queue;
        wait_queue = newP;
    }



}

void rem_wait(int processID){
    struct process *tempw;
    struct process *del;
    tempw = wait_queue;
    if(tempw->pid == processID){
        wait_queue = tempw->next;

    }
    else{
        while(tempw->next != NULL){
            if(tempw->next->pid == processID){
                del = tempw->next;
                tempw->next = del->next;
                free(del);
            }
            tempw = tempw->next;
        }
    }
}

void print(){
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("Allocation Queue:\n");
    block *temp;
    temp = allocation_queue;
    while(temp!= NULL){
        if(temp->hole)
            printf("Empty Block:\t %dKB \t Start Address: 0x%08x \t End Address: 0x%08x\n",temp->size_kb,temp->s_address,(temp->s_address + temp->size_kb-1));
        else
            printf("Process %d:\t %dKB \t Start Address: 0x%08x \t End Address: 0x%08x\n",temp->pid,temp->size_kb,temp->s_address,(temp->s_address + temp->size_kb-1));
        temp = temp->next;
    }
    printf("Input Queue:\n");
    process *tempw;
    tempw = wait_queue;
    while(tempw != NULL){
        printf("Process %d:\t %dKB\n",tempw->pid,tempw->p_size);
        tempw = tempw->next;
    }
    printf("--------------------------------------------------------------------------------------------------------\n");

}


