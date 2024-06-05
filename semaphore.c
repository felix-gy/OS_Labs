#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

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
    int id = getSem();
    controlSem(id, 4);

    int pid;
    pid = fork();
    srand(pid);
    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid)
    {
        char *s = "abcdefgh";
        int l = strlen(s);
        for (int i = 0; i < l; ++i)
        {
            semP(id);
            putchar(s[i]);
            fflush(stdout);
            sleep(rand() % 2);
            putchar(s[i]);
            fflush(stdout);
            semV(id);

            sleep(rand() % 2);
        }
    }
    else
    {
        char *s = "ABCDEFGH";
        int l = strlen(s);
        for (int i = 0; i < l; ++i)
        {
            semP(id);
            putchar(s[i]);
            fflush(stdout);
            sleep(rand() % 2);
            putchar(s[i]);
            fflush(stdout);
            semV(id);

            sleep(rand() % 2);
        }
    }
}