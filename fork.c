#include<stdio.h>

int main() {
   int pid;
   pid = fork();

   // Child process
   if (pid == 0) {
      system("echo 'hijo'");
      system("echo $$");
      sleep(100);
   } else {
      system("echo 'padre'");
      system("echo $$");
      sleep(100);
   }
   return 0;
}
