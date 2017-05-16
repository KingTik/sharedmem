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
sem_t *sem1;  
char *shm;
char *shm_rd;


void *producent(void *arg){

    time_t current_time;
    char time_now[30];
    char *user_data;
    user_data = (char*) malloc (sizeof(char)*30);

    while(1){
    //data i czas
    
        current_time = time(0);
        
        strftime (time_now, 30, "%m-%d %H:%M:%S", localtime (&current_time));
        
        //----------------
        
        printf("%s: ", (char *) arg);
        fgets(user_data, 29, stdin );
        sprintf(shm, "%s (%s) \n > %.30s",(char *) arg, time_now, user_data );
        sem_post(sem1);
    }



}

void *konsument(void *arg){
    char buff[90];
    while(1){
       
        
         if(strncmp(buff, shm, sizeof(buff)) || !strncmp(buff, arg, sizeof(arg)) ){
            strncpy(buff, shm, sizeof(buff));
            printf("\r%s", buff);
            putchar('\n');
            
            if(!strncmp(buff, arg, sizeof(arg)))
            printf("\r%s", arg);
         }
    }

}

int main(int argc, char* argv[]){


    if(argc <= 1){
        printf("usage: ./client <user_name>\n");
        exit(-1);
    }


    int shmid, i;
    key_t key;
    pthread_t tid[2];
    
    key = 5678;

    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    if ((shm_rd = shmat(shmid, NULL, SHM_RDONLY)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    sem1 = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 0);

    
    pthread_create( &tid[0], NULL, producent, (void *)argv[1]); 
    pthread_create( &tid[1], NULL, konsument, (void *)argv[1]); 
    
     for (i=0; i<2; i++){
         pthread_join(tid[i], NULL);
     }
    
    exit(0);



    return 0;
}








