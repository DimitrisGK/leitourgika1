#define MAX_MESSAGE_SIZE 15

void *thread_a(void *);
void *thread_b(void *);

struct SharedMemory{
    sem_t semaphore;        //semaphore for checking shared memory access
    int message_num;        //number of messages
    char messages[100][MAX_MESSAGE_SIZE];  //message table
};
