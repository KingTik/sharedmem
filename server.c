#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHMSZ     1024
#define SEM_PATH "/tmp"

int main(){


    int shmid_rd, shmid_wr;
    key_t key_in, key_out;
    char *shm_rd, *s, *shm_wr;
    sem_t *sem1;
    key_in = 5678;

    if ((shmid_rd = shmget(key_in, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm_rd = shmat(shmid_rd, NULL, SHM_RDONLY)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    sem1 = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 1);

    while(1){
        sem_wait(sem1);
        sleep(5);
        printf("%s", shm_rd);
        putchar('\n');
        
    }
  
    

    exit(0);




}