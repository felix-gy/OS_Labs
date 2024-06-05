#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//
#include <sys/sem.h>
#include <errno.h>
#define SHMSZ     27


#define KEY 0x2222

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

union semun u;

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

void semP(int id)
{
    if (semop(id, &p, 1) < 0)
    {
        perror("semop p");
        exit(13);
    }
}

void semV(int id)
{
    if (semop(id, &v, 1) < 0)
    {
        perror("semop p");
        exit(14);
    }
}

int getSem()
{
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    return id;
}

void controlSem(int id, int numRecursos)
{
    //union semun u;
    u.val = numRecursos;
    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }
}

int main()
{
    int id = getSem();
    controlSem(id, 4);
    //
    int shmid;
    key_t key;
    char *shm, *s;
    key = 5670;

    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    char lastData[SHMSZ] = "";
    char buffer[SHMSZ];

    while (1) {
        semP(id);
        strncpy(buffer, shm, SHMSZ);
        if (strncmp(lastData, buffer, SHMSZ) != 0) {
            // Convertimos el numero a entero 
            int value = atoi(buffer);
            sprintf(buffer, "%d, %d", value, value*2);
            
            // Lo ponermos en la Shared memory
            strncpy(shm, buffer, SHMSZ);

            //Actualizamos Last Data 
            strncpy(lastData, buffer, SHMSZ);
            printf("Producto:%s\n", shm);
        }   
        semV(id);
        //sleep(1);
    }
    exit(0);
}
