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


#define SHMSZ     27

int main()
{

    int shmid;
    key_t key;
    char *shm, *s;

    /*
     * We need to get the segment named
     * "5678", created by the server.
     */
    key = 5678;

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
    char buffer[SHMSZ];
    strncpy(buffer, shm, SHMSZ);
    strncpy(lastData, shm, SHMSZ);
    while (1) {
        if (strncmp(lastData, buffer, SHMSZ) != 0) {
            printf("print:%s\n", shm);
            strncpy(lastData, buffer, SHMSZ);
        }
        strncpy(buffer, shm, SHMSZ);
    }
    exit(0);
}