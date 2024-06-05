#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void sigchld_handler(int sig);
int contador=0;
int signalON;
int main()
{
    signalON = 0;
    signal(SIGINT, sigchld_handler);
    int fd;
    char * myfifo = "/tmp/myfifo3";

    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);

    /* write random numer to the FIFO */
    fd = open(myfifo, O_WRONLY);
    for(;;){
        if (signalON){
            signalON=0;
            char st[20];
            int r = rand()%999 ;
            sprintf(st, "%d", r);
            printf("Envio %d \n", r);
            write(fd, st, sizeof(st));
            contador = contador + 1;
            if (contador >= 10) { 
                exit(0);
            }
        }

    }
    close(fd);
    /* remove the FIFO */
    unlink(myfifo);
    return 0;
}


void sigchld_handler(int sig){
    printf("Señal recibida \n");
    signalON = 1;
}
