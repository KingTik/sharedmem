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
#define USER_MESSAGE_LENGTH 60 //dozwolona dlugosc wiadomosci





struct Message{
    char current_time[17];  //czas nadania wiadomosci
    char username[15];      // nazwa uzytkownika
    char message[USER_MESSAGE_LENGTH]; //wiadomosc
    short int recieved;     //informacja czy wiadomosc zostala odebrana 0-nie 1-tak
    unsigned int message_id;    //id wiadomosci; jest nadawane przez server
}Message;

struct Message *shm;



void* display(void *arg){
    key_t key = 8769;
    struct Message *list;
    int i, shmid;
    int last_id=0;
    // pamiec wspoldzielona dla listy wiadomosci wystawianej przez serwer
    // zawiera wiadomosci od wszystkich uzytkownikow
    if ((shmid = shmget(key, sizeof(struct Message), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((list = (struct Message *) shmat(shmid, NULL, 0)) == (struct Message *) -1) {
        perror("shmat");
        exit(1);
    }

    //czeka z odswiezeniem az bedzie nowa wiadomosc
    // sprawdza na podstawie id wiadomosci ktory nadaje server
    while(1){
        if(last_id != list[0].message_id){
            last_id = list[0].message_id;
            system("clear");
            for(i=9; i>=0; i--){    //wyswietlanie 10 ostatnich wiadomosci
                if( strcmp(list[i].username,"") ){ //sprawdza czy struktura jest czyms wypelniona zeby nie wyswietlac smieci
                    printf("%s\n", list[i].current_time);
                    printf(">%s: %s\n", list[i].username, list[i].message);
                }
            }
        }else{ //nie ma nowych wiadomosci
            sleep(0.5);
        }
    }
}

int main(int argc, char* argv[]){

    //uzytkownik nie podla nazwy
    if(argc <= 1){
        printf("usage: ./client <user_name>\n");
        exit(-1);
    }
    //uzytkownik podal zbyt dluga nazwe
    if(strlen(argv[1]) > 14){
        printf("username too long! (up to 15 characters) \n");
        exit(-1);
    }

    

    pthread_t tid;
    int shmid, i;
    key_t key;
    time_t current_time;
    sem_t *sem_msg;  
    struct Message *wiadomosc;
    
    
    key = 5278;
    // pamiec wspoldzielona dla wiadomosci wychodzacej
    if ((shmid = shmget(key, sizeof(struct Message), IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }
    if ((wiadomosc = (struct Message *) shmat(shmid, NULL, 0)) == (struct Message *) -1) {
        perror("shmat");
        exit(1);
    }

    
    // semafor mowiacy ze nowa wiadomosc zostala wyslana
    sem_msg = sem_open(SEM_PATH, O_CREAT, S_IRUSR | S_IWUSR, 0);
    
    //watek odpowiedzialny za wyswietlanie poprzednich wiadomosci tego i innych uzytkownikow
    pthread_create(&tid, NULL, display, NULL);
    

    while (1){ //glowna petla

        
        wiadomosc->recieved = 0;                                                                // wiadomosc nieodebrana
        // najpierw jest odczytanie inputu a pozniej wypelnienie struktury zeby zminimalizowac
        // mozliwosc 2 uzytkownikow nadpisania sobie wiadomosci i przemieszania danych w strukturze

        fgets(wiadomosc->message,USER_MESSAGE_LENGTH, stdin );                                  //input uzytkownika
        current_time = time(0);                                                                 //czas nadania wiadomosci
        strftime (wiadomosc->current_time, 17, "%m-%d %H:%M:%S", localtime (&current_time));
        strncpy(wiadomosc->username, argv[1], sizeof(wiadomosc->username));                     //nazwa uzytkownika
        
        

        sem_post(sem_msg);

        while(wiadomosc->recieved != 1){
            //czekanie na potwiedzenie dotarcia wiadomosci
        }

        
    }
    
     
    
    exit(0);



    return 0;
}








