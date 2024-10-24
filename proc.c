#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include "functions.h"


int main(){
    struct SharedMemory *shm = (struct SharedMemory *)malloc(sizeof(struct SharedMemory));  //creating share memory
    sem_init(&(shm->semaphore), 1, 1);  //initialize the shemaphore
    shm->message_num=0;

    //creating thrads for proccesses a,b
    pthread_t thrd_a, thrd_b;
    pthread_create(&thrd_a, NULL, thread_a, shm);
    pthread_create(&thrd_b, NULL, thread_b, shm);

    pthread_join(thrd_a, NULL); //waiting thread a to terminate
    pthread_join(thrd_b, NULL); //waiting thread b to terminate
    
    sem_destroy(&(shm->semaphore));
    free(shm);      //free share memory

    return 0;
}