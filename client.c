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

#define SHMSZ 1024
#define SEM_PATH "/tmp"


int main(int argc, char* argv[]){


    if(argc <= 1){
        printf("usage: ./client <user_name>\n");
        exit(-1);
    }

    time_t current_time;
    char data[] = "loerm ipsum bla";
    char *user_data;
    char time_now[30];
    char c;
    int shmid;
    key_t key;
    char *shm, *s;
    sem_t *sem1;   
    
    key = 5678;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    user_data = (char*) malloc (sizeof(char)*30);
    
    sem1 = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);

    while(1){
    //data i czas
    current_time = time(0);
    strftime (time_now, 30, "%m-%d %H:%M:%S", localtime (&current_time));
    //----------------
    
    printf("%s: ", argv[1]);
    fgets(user_data, 29, stdin );
    sprintf(shm, "%s (%s) \n > %s",argv[1], time_now, user_data );
    sem_post(sem1);
    }
     
    
    exit(0);



    return 0;
}








