#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
///
#include <signal.h>	//	signal()
#include <sys/wait.h>
#include <sys/time.h>
#include <fcntl.h>

#define MAXSIZE 128

void die(char *s) {
    perror(s);
    exit(1);
}

struct msgbuf {
    long mtype;
    char mtext[MAXSIZE];
};

void sign_handler_INT(int);
int signalON = 0;

int main() {
    signal(SIGINT,sign_handler_INT);
    pid_t pid;
    int msqid_1;
    int msqid_2;
    int msgflg_1 = IPC_CREAT | 0660;
    int msgflg_2 = IPC_CREAT | 0606;
    key_t key_1 = 101010;
    key_t key_2 = 151515;
    struct msgbuf sbuf;
    size_t buflen;

    srand(time(NULL));  // Inicializar la generación de números aleatorios

    // Key 101010 - 0660
    if ((msqid_1 = msgget(key_1, msgflg_1)) < 0) {
        die("msgget");
    }
    // Key 151515 - 0606
    if ((msqid_2 = msgget(key_2, msgflg_2)) < 0) {
        die("msgget");
    }
    //printf("PID: %d\n", pid);
    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    // num of messages to send
    //int rep = 100;
    //Generacion de numeros aleatorios del 1 al 100
    int num = 100;

    while (1)
    {
        if (signalON)
        {
            signalON=0;
            if (pid == 0) { // Proceso hijo

                for (int i = 0; i < 100; i++) {
                
                    int random_number = rand() % num + 1; // Números del 1 al 10

                    sprintf(sbuf.mtext, "%d Soy papá y genero el num: %d", i+1, random_number);
                    buflen = strlen(sbuf.mtext) + 1;
                    sbuf.mtype = random_number; // Usar número aleatorio como mtype
                    
                    if (random_number  <= 10) // MENORES a 10 
                    {

                        if (msgsnd(msqid_1, &sbuf, buflen, 0) < 0) { // Uso de bloqueo para asegurar el envío
                            printf("%d, %ld, %s, %d\n", msqid_1, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }                        
                    
                    }
                    else  // MAYORES A 10
                    {
                        if (msgsnd(msqid_2, &sbuf, buflen, 0) < 0) { // Uso de bloqueo para asegurar el envío
                            printf("%d, %ld, %s, %d\n", msqid_2, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }
                    }
                }
                printf("1000 mensajes del HIJO enviados.\n");
            } else {  // Proceso padre
                
            
                for (int i = 0; i < 100; i++) {
                    int random_number = rand() % num + 1; // Números del 1 al 10

                    sprintf(sbuf.mtext, "%d Soy hijo y genero el num: %d", i+1, random_number);
                    buflen = strlen(sbuf.mtext) + 1;
                    sbuf.mtype = random_number; // Usar número aleatorio como mtype

                    if (random_number  <= 10) // MENORES a 10 
                    {

                        if (msgsnd(msqid_1, &sbuf, buflen, 0) < 0) { // Uso de bloqueo para asegurar el envío
                            printf("%d, %ld, %s, %d\n", msqid_1, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }                        
                    
                    }
                    else  // MAYORES A 10
                    {
                        if (msgsnd(msqid_2, &sbuf, buflen, 0) < 0) { // Uso de bloqueo para asegurar el envío
                            printf("%d, %ld, %s, %d\n", msqid_2, sbuf.mtype, sbuf.mtext, (int)buflen);
                            die("msgsnd");
                        }
                    }
                }
                printf("1000 mensajes del PADRE enviados.\n");
            }
            exit(0);
        }
    }
}

void sign_handler_INT(int signum)
{
	signalON=1;
}