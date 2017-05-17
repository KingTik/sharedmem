#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>




#define SHMSZ 2048
#define SEM_PATH "/tmp"
#define USER_MESSAGE_LENGTH 60
#define QUEUE_SIZE 10

struct Message{
    char current_time[17];
    char username[20];
    char message[USER_MESSAGE_LENGTH];
    short int recieved;
}Message;


void queue_add(struct Message *wiad, struct Message *queue){

    int i;
    for(i = QUEUE_SIZE-1; i>0; i--){
        queue[i]= queue[i-1];
    }
    strncpy(queue[0].username,wiad->username, sizeof(queue[0].username) );
    strncpy(queue[0].current_time,wiad->current_time, sizeof(queue[0].current_time) );
    strncpy(queue[0].message,wiad->message, sizeof(queue[0].message) );
}




int main(){


    int shmid, shmid_wr, i;
    key_t key_in, key_out;
    char *shm_rd, *s, *shm_wr;
    sem_t *sem1;
    key_in = 5678;
    struct stat st = {0};
    struct Message *shm;
    struct Message *queue;
    queue = (struct Message*) malloc (sizeof(struct Message)*QUEUE_SIZE);

    if ((shmid = shmget(key_in, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = (struct Message *) shmat(shmid, NULL, 0)) == (struct Message *) -1) {
        perror("shmat");
        exit(1);
    }
    //shm = (struct Message *) shmat(shmid, NULL, 0);
    sem1 = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);

    while(1){
        sem_wait(sem1);
        queue_add(shm, queue);
        shm->recieved = 1; //odebrano
        system("clear");

        //wyswietlanie
        for(i=QUEUE_SIZE-1; i>=0; i--){
            if( strcmp(queue[i].username,"") ){
                printf("%s\n", queue[i].current_time);
                printf(">%s: %s\n", queue[i].username, queue[i].message);
            }
        }
       
        
        
        
    }
  
    

    exit(0);




}