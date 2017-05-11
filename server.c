#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ     27
#define SEM_PATH "/tmp"

int main(){


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

    sem1 = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);

    while(1){
        sem_wait(sem1);
        printf("%s", shm);
        putchar('\n');
        
    }
  
    *shm = '*';

    exit(0);




}