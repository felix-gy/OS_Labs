/*
 * shm-client - client program to demonstrate shared memory.
 */
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

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5670;

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    /*
     * Now we attach the segment to our data space.
     */
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    char t[SHMSZ] = "";
    strncpy(shm, t, SHMSZ);
    
    char lastData[SHMSZ];
    char *buffer =shm;
    strncpy(lastData, shm, SHMSZ);

    while (1) {
        semP(id);
        if (strncmp(lastData, buffer, SHMSZ) != 0) {
            printf("print:%s\n", shm);
            strncpy(lastData, buffer, SHMSZ);
        }
        semV(id);
    }
    exit(0);
}