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

    /*
     * Locate the segment.
     */
    if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
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
    
    char lastData[SHMSZ];
    char buffer[SHMSZ];

    // Colocamos 100
    char t[SHMSZ] = "100";
    strncpy(shm, t, SHMSZ);
    strncpy(lastData, shm, SHMSZ);

    //strncpy(buffer, shm, SHMSZ);
    //strncpy(lastData, buffer, SHMSZ);
    while (1) {
        semP(id);
        strncpy(buffer, shm, SHMSZ);
        if (strncmp(lastData, buffer, SHMSZ) != 0) {
            // Aumentados 1 a la secuencia

            int value = atoi(buffer);
            //printf("value: %d \n", value);
            value++;
            if (value == 1001)
            {
                semV(id);
                break;
            }
            
            sprintf(buffer, "%d", value);
            
            strncpy(shm, buffer, SHMSZ);

            // Actualizamos Last Data
            strncpy(lastData, shm, SHMSZ);

            // Imprimimos El resultado
            printf("Secuencia:%s\n", shm);
        }
        semV(id);
        //sleep(1);
        //Leemos 
        
    }
    exit(0);
}