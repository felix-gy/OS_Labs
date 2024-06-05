#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define KEY 0x1111

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};
struct sembuf v = {0, +1, SEM_UNDO};

int main()
{

    int shmid;
    key_t key;
    char * shm;
    int id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (id < 0)
    {
        perror("semget");
        exit(11);
    }
    union semun u;
    u.val = 1;

    if ((shmid = shmget(key, 20, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    if ((shm = shmat(id, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    if (*shm != 'm' && *shm != 'M')
    {
        *shm = 'm' ;
    }

    if (semctl(id, 0, SETVAL, u) < 0)
    {
        perror("semctl");
        exit(12);
    }

        char *s = "abcdefgh";
        int l = strlen(s);
        for (int i = 0; i < l; ++i)
        {
            if (semop(id, &p, 1) < 0)
            {
                perror("semop p");
                exit(13);
            }
            if (*shm == 'm')
            {
                putchar(s[i]);
                fflush(stdout);
                sleep(rand() % 2);
                putchar(s[i]);
                fflush(stdout);
                *shm = 'M';
            }
            else{
                i--;
            }
            
            
            if (semop(id, &v, 1) < 0)
            {
                perror("semop p");
                exit(14);
            }

            sleep(rand() % 2);
        }
    
}