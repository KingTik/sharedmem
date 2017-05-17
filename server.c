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





#define SEM_PATH_MSG "/msg"
#define USER_MESSAGE_LENGTH 60
#define QUEUE_SIZE 10

struct Message{
    char current_time[17];
    char username[20];
    char message[USER_MESSAGE_LENGTH];
    short int recieved;
    unsigned int message_id;
}Message;

unsigned int message_id_cnt =1; //id wiadomosci 

//dodanie nowego elementu na poczatek tablicy i przesuniecie wszystkich pozostalych o jeden dalej
void queue_add(struct Message *wiad, struct Message *queue){

    int i;
    for(i = QUEUE_SIZE-1; i>0; i--){ //przesuniecie elementow
        queue[i]= queue[i-1];
    }
    //wstawienie nowego na poczatek tablicy
    strncpy(queue[0].username,wiad->username, sizeof(queue[0].username) );
    strncpy(queue[0].current_time,wiad->current_time, sizeof(queue[0].current_time) );
    strncpy(queue[0].message,wiad->message, sizeof(queue[0].message) );
    queue[0].message_id = message_id_cnt;
    message_id_cnt += 1;
}




int main(){


    int shmid, shmid2, shmid_wr, i;
    key_t key_in, key_out;
    char *shm_rd, *s, *shm_wr;
    sem_t *sem_msg;
    key_in = 5678;
    key_out = 8769;
    struct stat st = {0};
    struct Message *shm;
    struct Message *queue;
    
    

    //pamiec wspoldzielona dla wiadomosci przychodzacej
    if ((shmid = shmget(key_in, sizeof(struct Message), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = (struct Message *) shmat(shmid, NULL, 0)) == (struct Message *) -1) {
        perror("shmat");
        exit(1);
    }

        //pamiec wspoldzielona dla wiadomosci wychodzacych
    if ((shmid2 = shmget(key_out, sizeof(struct Message)*QUEUE_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget_out");
        exit(1);
    }
    if ((queue = (struct Message *) shmat(shmid2, NULL, 0)) == (struct Message *) -1) {
        perror("shmat_out");
        exit(1);
    }


    //semafor dla wiadomosci wychodzacej
    sem_msg = sem_open(SEM_PATH_MSG, O_CREAT, S_IRUSR | S_IWUSR, 1);

    
    

    while(1){

        sem_wait(sem_msg);
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