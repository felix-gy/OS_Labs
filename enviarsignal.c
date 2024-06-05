#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char * argv [])
{
	int signal;
	pid_t pid = atoi(argv[1]);
	pid_t pid_2 = atoi(argv[2]);
	kill(pid, 2);
	kill(pid_2, 2);
	return 0;
}
