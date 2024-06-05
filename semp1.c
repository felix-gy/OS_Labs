#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
//
#include <sys/shm.h>
#include <string.h>

#define KEY 0x1111

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
    /// Shared memory M or m
    int shmid;
    key_t key;
    char *shm;
    key = 5678;

    if ((shmid = shmget(key, 20, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    //
    *shm = 'm' ;
    //

    /// Shared memory 2
    int shmid_2;
    key_t key_2;
    char *shm_2;
    key_2 = 5679;

    if ((shmid_2 = shmget(key_2, 20, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm_2 = shmat(shmid_2, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    ///
    char t[20] = "";
    strncpy(shm_2, t, 20);
    /// 

    int id = getSem();
    controlSem(id, 4);
    char *s = "abcdefgh";
    int l = strlen(s);
    for (int i = 0; i < l; ++i)
    {
        semP(id);
        if (*shm == 'm')
        {
            *shm_2 = s[i];
            //sleep(rand() % 2);
            *(shm_2+1) = s[i];
            shm_2 = shm_2+4;
            //
            *shm = 'M';
        }
        else
        {
            i--;
        }
        
        semV(id);

        //sleep(rand() % 2);
    }

}
