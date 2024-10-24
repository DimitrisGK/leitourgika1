#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "functions.h"


//this threads send a message
void *thread_a(void *shared_memory){
    struct SharedMemory *shm = (struct SharedMemory *)shared_memory;

    while (1){
        char users_input[MAX_MESSAGE_SIZE];     //table for store the messages
        printf("Enter message or #BYE#:");
        fgets(users_input, MAX_MESSAGE_SIZE, stdin);    //user entering a message
        // users_input[strcspn(users_input, "\n")] = '\0';  //remove "\n" from users_input

        if (strcmp(users_input, "#BYE#")==0){  //if "BYE" stop the thread
            break;
        }

        //put message in share memory
        sem_wait(&(shm->semaphore));
        strcpy(shm->messages[shm->message_num], users_input);
        shm->message_num++;
        sem_post(&(shm->semaphore));
    }
    pthread_exit(NULL);
}

//this thread recieve a message
void *thread_b(void *shared_memory){
    struct SharedMemory *shm = (struct SharedMemory *)shared_memory;
    struct timeval start, end;
    int num_of_messages=0;
    int num_of_pieces=0;   //number of the pieces that they send
    long sum_time=0;

    while (1){
        gettimeofday(&start, NULL);
        sem_wait(&(shm->semaphore));    //getting the message from process A
        if (shm->message_num > 0){
            num_of_messages++;
            num_of_pieces+=strlen(shm->messages[0]);   
            gettimeofday(&end, NULL);
            long elapsed_time=(end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
            sum_time+=elapsed_time;

            printf("Process B received: %s\n", shm->messages[0]);
            for (int i = 0; i < shm->message_num - 1; i++){     //remove message from shared memmory
                strcpy(shm->messages[i], shm->messages[i+1]);
            }
            shm->message_num--;

            sem_post(&(shm->semaphore));
        }else{
            sem_post(&(shm->semaphore));
            usleep(100000);     //waiting before checking for a message again
        }
    }
    printf("Number of messages that received and send: %d\n", num_of_messages);
    printf("Number of pieces that received: %d\n", num_of_pieces);
    printf("Average number of pieces that received per message: %d\n", num_of_pieces/num_of_messages);
    printf("Average waiting time for first piece: %ld\n", sum_time/num_of_messages);

    pthread_exit(NULL);
}