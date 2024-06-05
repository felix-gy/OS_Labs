#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Incluir para funciones como sleep
#include <errno.h>
#define MAXSIZE 128

void die(char *s) {
    perror(s);
    exit(1);
}

struct msgbuf {
    long mtype;          
    char mtext[MAXSIZE]; // Mensaje recibido
};

int main() {
    int msqid;
    key_t key = 151515;
    struct msgbuf rcvbuffer;

    if ((msqid = msgget(key, 0666)) < 0)
        die("msgget()");

    // Intentar recibir mensajes mayores a 10
    for (int mtype = 11; mtype <= 100; mtype++) {
        while (1) {
            int result = msgrcv(msqid, &rcvbuffer, MAXSIZE, mtype, IPC_NOWAIT);
            if (result >= 0) {
                printf("Mensaje recibido: %s\n", rcvbuffer.mtype, rcvbuffer.mtext);
            } else {
                if (errno == ENOMSG) {
                    break;  // No more messages of this type
                } else {
                    die("msgrcv failed");
                }
            }
        }
    }
    return 0;
}
