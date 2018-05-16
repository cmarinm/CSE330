/*
Author: Cesar Marin
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#include "threads.h"
TCB_t * execution_q;            //Global execution queue

void sleep(int sec);

void function1()          //First thread will run on this function
{
    int i=0;
    for(i=0;i<4;i++)
    {
        printf("Task 1 is running\n");
        sleep(2);    //Delay of 2 sec
        yield();     //yield to pause currently executing thread temporarily & allow other threads to execute

    }
}

void function2()
{
    int i=0;
    for(i=0;i<4;i++)
    {
        printf("Task 2 is running\n");
        sleep(2);    //Delay of 2 sec
        yield();     //yield to pause currently executing thread temporarily & allow other threads to execute

    }
}

void function3()
{
    int i=0;
    for(i=0;i<4;i++)
    {
        printf("Task 3 is running\n");
        sleep(2);    //Delay of 2 sec
        yield();     //yield to pause currently executing thread temporarily & allow other threads to execute

    }
}

void main()
{

    // Initialize execution queue
    InitQueue(execution_q);

    // Create threads to run on functions 1, 2 and 3
    create_thread((void (*)(void *)) function1);
    create_thread((void (*)(void *)) function2);
    create_thread((void (*)(void *)) function3);

    // run the execution queue
    run();

 printf("@__PROGRAM_END__@\n");

}

void sleep(int sec){
 struct timespec tim, tim2;
    tim.tv_sec = sec;
    nanosleep(&tim,&tim2);
}
