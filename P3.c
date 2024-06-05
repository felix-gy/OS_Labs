#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
void sigchld_handler(int sig);
int flag  = 0;
int main(int argc, char *argv [])
{
    signal(SIGINT, sigchld_handler);
    int counter = 0;
    while (1)
    {
        if (flag)
        {
            flag = 0;
            int processID = atoi(argv[1]);
            printf("Señal enviada \n");
            kill(processID,2);
        }
    }
	return 0;
}

void sigchld_handler(int sig)
{
    flag = 1;
}
