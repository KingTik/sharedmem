#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ 2048
#define SEM_PATH "/tmp"
#define USER_MESSAGE_LENGTH 60

sem_t *sem1;  

char *shm_rd;

struct Message{
    char current_time[17];  //czas nadania wiadomosci
    char username[20];      // nazwa uzytkownika
    char message[USER_MESSAGE_LENGTH]; //wiadomosc
    short int recieved;     //informacja czy wiadomosc zostala odebrana 0-nie 1-tak
}Message;

struct Message *shm;



int main(int argc, char* argv[]){


    if(argc <= 1){
        printf("usage: ./client <user_name>\n");
        exit(-1);
    }


    int shmid, i;
    key_t key;
    time_t current_time;
    char time_now[14];
    struct Message *wiadomosc;
    
    
    key = 5678;

    if ((shmid = shmget(key, sizeof(struct Message), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((wiadomosc = (struct Message *) shmat(shmid, NULL, 0)) == (struct Message *) -1) {
        perror("shmat");
        exit(1);
    }

   
    sem1 = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 0);

    
    

    while (1){

        wiadomosc->recieved = 0;
        fgets(wiadomosc->message,USER_MESSAGE_LENGTH, stdin );
        strftime (wiadomosc->current_time, 17, "%m-%d %H:%M:%S", localtime (&current_time));
        strncpy(wiadomosc->username, argv[1], 20);
        //shm = wiadomosc;
        sem_post(sem1);

        while(wiadomosc->recieved != 1){
            
        }

        
    }
    
     
    
    exit(0);



    return 0;
}








