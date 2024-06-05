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

    key = 5678;

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

    strncpy(buffer, shm, SHMSZ);
    strncpy(lastData, buffer, SHMSZ);
    while (1) {
        strncpy(buffer, shm, SHMSZ);
        if (strncmp(lastData, buffer, SHMSZ) != 0) {

            // Aumentados 1 a la secuencia
            int value = atoi(buffer);

            //printf("value: %d \n", value);
            value++;
            if (value == 1001)
            {
                break;
            }
            
            sprintf(buffer, "%d", value);
            
            strncpy(shm, buffer, SHMSZ);

            // Actualizamos Last Data
            strncpy(lastData, shm, SHMSZ);

            // Imprimimos El resultado
            printf("Secuencia:%s\n", shm);
        }
        //sleep(1);

        //Leemos 
        
    }
    exit(0);
}