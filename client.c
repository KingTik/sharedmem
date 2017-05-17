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


#define SEM_PATH "/msg"
#define USER_MESSAGE_LENGTH 60





struct Message{
    char current_time[17];  //czas nadania wiadomosci
    char username[20];      // nazwa uzytkownika
    char message[USER_MESSAGE_LENGTH]; //wiadomosc
    short int recieved;     //informacja czy wiadomosc zostala odebrana 0-nie 1-tak
    unsigned int message_id;
}Message;

struct Message *shm;



void* display(void *arg){
    key_t key = 8769;
    struct Message *list;
    int i, shmid;
    int last_id=0;
    if ((shmid = shmget(key, sizeof(struct Message), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((list = (struct Message *) shmat(shmid, NULL, 0)) == (struct Message *) -1) {
        perror("shmat");
        exit(1);
    }

    while(1){
        if(last_id != list[0].message_id){
            last_id = list[0].message_id;
            system("clear");
            for(i=9; i>=0; i--){
                if( strcmp(list[i].username,"") ){
                    printf("%s\n", list[i].current_time);
                    printf(">%s: %s\n", list[i].username, list[i].message);
                }
            }
        }else{
            sleep(0.5);
        }
    }
}

int main(int argc, char* argv[]){


    if(argc <= 1){
        printf("usage: ./client <user_name>\n");
        exit(-1);
    }

    pthread_t tid;
    int shmid, i;
    key_t key;

    time_t current_time;
    sem_t *sem_msg;  
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

    
   
    sem_msg = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 0);
    
    
    pthread_create(&tid, NULL, display, NULL);
    current_time = time(0);

    while (1){

        wiadomosc->recieved = 0;
        fgets(wiadomosc->message,USER_MESSAGE_LENGTH, stdin );
        current_time = time(0);
        strftime (wiadomosc->current_time, 17, "%m-%d %H:%M:%S", localtime (&current_time));
        strncpy(wiadomosc->username, argv[1], 20);
        
        

        sem_post(sem_msg);

        while(wiadomosc->recieved != 1){
            //czekanie na potwiedzenie dotarcia wiadomosci
        }

        
    }
    
     
    
    exit(0);



    return 0;
}








