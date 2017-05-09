#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#define SHMSZ 1024


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
    
    current_time = time(0);
    strftime (time_now, 30, "%m-%d %H:%M:%S", localtime (&current_time));
    
    
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
    
    s = shm;
    printf("%s: ", argv[1]);
    scanf("%30s", user_data);
    sprintf(shm, "%s (%s) \n > %s",argv[1], time_now, user_data );

     
    while (*shm != '*')
        sleep(1);

    exit(0);



    return 0;
}








