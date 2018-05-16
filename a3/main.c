#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
int a[20] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19};
int b[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
int c[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int sum = 0;
int p_index = 0;
int c_index = 0;
pthread_t prod1;
pthread_t prod2;
pthread_t cons;
sem_t producers;
sem_t prod_cons;

void timer(int sec);

void* producer(void *arg){
    while(sum <= 200){
        sem_wait(&producers);
        sem_wait(&prod_cons);






        if(pthread_equal(pthread_self(),prod1)) //check which producer is running
            printf("producer 1 has written ");
        else
            printf("producer 2 has written ");


        c[p_index] = a[p_index] + b[p_index];   //update c, update global sum, then print statement
        sum = sum + c[p_index];
        printf("c[%d] and updated global sum\n\n",p_index);
        p_index++;


        sem_post(&prod_cons);                   //signal semaphores
        timer(1);                               //wait for 1 second
        sem_post(&producers);
        timer(1);

    }
    printf("Sum value has exceeded 200\n\n");

}

void* consumer(void *arg){
    while(sum <= 200){
        sem_wait(&prod_cons);
        if(sum == 0);           //make sure that the very first run is not consumed before producers produce 1st var
        else{
            printf("consumer has read c[%d] = %d\n",c_index,c[c_index]);
            printf("consumer has read read global sum = %d\n\n",sum);
            c_index++;


            sem_post(&prod_cons);
            timer(1);


        }


    }
    printf("Sum value has exceeded 200\n");
    while(c_index < p_index){
        printf("consumer has read c[%d] = %d\n",c_index,c[c_index]);
        printf("consumer has read read global sum = %d\n",sum);
        c_index++;
        timer(1);
    }

}

int main() {

    //initialize semaphores
    sem_init(&producers,0,1); //there can only be 1 producer running at a time
    sem_init(&prod_cons,0,2); //allows for 2 values to be produced before consumed
    //create threads
    pthread_create(&prod1,NULL,&producer,NULL);
    pthread_create(&prod2,NULL,&producer,NULL);
    pthread_create(&cons,NULL,&consumer,NULL);

    //wait for threads to join
    pthread_join(prod1,NULL);
    pthread_join(prod2,NULL);
    pthread_join(cons,NULL);

    sem_destroy(&producers);
    sem_destroy(&prod_cons);
    printf("---PROGRAM_END---\n");
    return 0;;

}


void timer(int sec){
    struct timespec tim, tim2;
    tim.tv_sec = sec;

    nanosleep(&tim,&tim2);
}

