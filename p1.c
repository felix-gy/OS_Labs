
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHMSZ 10

int main(int argv, char**argc)
{
	int shmid;
	key_t key;
	char *shm;
	key = 5671;

	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
	{
		perror("shmget error\n");
		exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char*)(-1))
	{
		perror("shmat error");
		exit(1);
	}
	char *s = shm;
	char *lastData = malloc(SHMSZ);
	strcpy(lastData, s);
	char *end = malloc(SHMSZ);

	// Open semaphore
	sem_t *semaphore = sem_open("/my_semaphore", O_CREAT, 0666, 1);
	if (semaphore == SEM_FAILED)
	{
		perror("sem_open error");
		exit(1);
	}

	while (strcmp(end, s) != 0)
	{
		// Lock semaphore
		if (sem_wait(semaphore) == -1)
		{
			perror("sem_wait error");
			exit(1);
		}

		if ((strcmp(lastData, s) != 0))
		{
			printf("Detect change to data -> %s\n", s);
			strcpy(lastData, s);
		}
		else
		{
			printf("s currently is %s\n", s);
			//sleep(1);
		}

		// Unlock semaphore
		if (sem_post(semaphore) == -1)
		{
			perror("sem_post error");
			exit(1);
		}
		
		// Wait for a short period before checking again
		usleep(100000);
	}
	free(lastData);
	free(end);
	return 0;
}
