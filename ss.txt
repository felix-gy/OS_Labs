#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void int2(int);
void F_USR2(int);
void F_USR1(int);
int u_cont_signal = 0;

int main () {
   signal(SIGINT, int2);
   signal(SIGUSR2, F_USR2);
   signal(SIGUSR1, F_USR1);
   while(1) {
      printf("Going to sleep for a second...\n");
      sleep(1); 
   }
   return(0);
}

void int2(int signum) {
	u_cont_signal++;
	printf("SO - %d \n",u_cont_signal);

}
void F_USR1(int signum) {
	u_cont_signal++;
	printf("Arquitectura - %d \n",u_cont_signal);
}
void F_USR2(int signum) {
	u_cont_signal++;
       	printf("Redes - %d \n",u_cont_signal);
}
