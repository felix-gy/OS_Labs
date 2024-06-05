#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>

int c;
int flag = 0;
void funcion_signal(int);

int main(void)
{
	signal(SIGINT, funcion_signal);
        int     pipe1[2], pipe2[2], nbytes;
        pid_t   childpid;
        char    readbuffer[80];
        int c;

        pipe(pipe1);
        pipe(pipe2);

        printf("Main program: \n"); 
        
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
        
       	while(1)
	{
		if(flag)
		{
                        flag = 0;
                        if(childpid == 0) //Child
                        {
                                int num;
                                /* Child process closes up input side of pipe */
                                for (int i = 0; i < 10; i++)
                                {
                                        //printf("Child program: \n");
                                        close(pipe1[1]); // SE CIERAA EL OUTPOUT 
                                        nbytes = read(pipe1[0], readbuffer, sizeof(readbuffer));
                                        readbuffer[nbytes] = '\0';
                                        printf("%i Child     Received: %s \n", i, readbuffer);
                                        num = atoi(readbuffer);


                                        // Calculo +3
                                        num = num+3;
                                        printf("%i number + 3 \n", i);

                                        sprintf(readbuffer, "%d", num);
                                        close(pipe2[0]);
                                        printf("%i Child     Sended: %s \n", i,readbuffer);
                                        write(pipe2[1], readbuffer, (strlen(readbuffer)+1));
                                        
                                        /* code */
                                }
                                
                        }
                        else //Parent
                        {
                                int number = rand()%99;
                                
                                char st[80];
                                printf("Random Number: %i \n", number);

                                printf("\n ----------------------------------------------\n");
                                // printf("Parent program:\n" );
                                close(pipe1[0]);
                                sprintf(st, "%d", number);
                                printf("0 Parent    Sended: %s \n",st);
                                write(pipe1[1], st, (strlen(st)));
                                /* Parent process closes up output side of pipe */
                                
                                close(pipe2[1]); // SE CIERAA EL OUTPOUT 
                                nbytes = read(pipe2[0], readbuffer, sizeof(readbuffer));
                                readbuffer[nbytes] = '\0';
                                printf("0 Parent    Received: %s \n",readbuffer);
                                number = atoi(readbuffer);
                                number = number + 7;
                                printf("0 number + 7 \n");
                                
                                for (int i = 1; i < 10; i++)
                                {
                                        printf("\n ----------------------------------------------\n");
                                        char st[20];
                                        // printf("Parent program:\n" );
                                        close(pipe1[0]);
                                        sprintf(st, "%d", number);
                                        printf("%i Parent    Sended: %s \n",i,st);
                                        write(pipe1[1], st, (strlen(st)));
                                        /* Parent process closes up output side of pipe */


                                        close(pipe2[1]); // SE CIERAA EL OUTPOUT 
                                        nbytes = read(pipe2[0], readbuffer, sizeof(readbuffer));
                                        readbuffer[nbytes] = '\0';
                                        printf("%i Parent    Received: %s \n", i,readbuffer);
                                        number = atoi(readbuffer);
                                        number = number + 7;
                                        printf("%i number + 7  \n", i);
                                }
                                printf("\nNumber: %i \n", number);
                        }
		}
	}	

        return(0);
//voltear 
}

void funcion_signal(int signum)
{
	flag = 1;
}
