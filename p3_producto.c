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
        strncpy(buffer, shm, SHMSZ);
        if (strncmp(lastData, buffer, SHMSZ) != 0) {
            
            // Si tiene coma obtener el contenido antes de la coma 
            char *commaPos = strchr(shm, ',');
            if (commaPos != NULL) {
                int length = commaPos - shm;
                strncpy(buffer, shm, length);
                buffer[length] = '\0'; // Null terminate the string
            }

            // Convertimos el numero a entero 
            int value = atoi(buffer);
            sprintf(buffer, "%d, %d", value, value*2);
            
            // Lo ponermos en la Shared memory
            strncpy(shm, buffer, SHMSZ);

            //Actualizamos Last Data 
            strncpy(lastData, buffer, SHMSZ);

            printf("Producto:%s\n", shm);
            
        }
        //sleep(1);
    }
    exit(0);
}
