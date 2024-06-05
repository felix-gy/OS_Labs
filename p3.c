
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHMSZ 10

int main()
{
	int shmid;
	key_t key;
	char *shm;
	char *s;
	key = 5671;

	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0)
	{
		perror("shmget error");
		exit(1);
	}
	if ((shm = shmat(shmid, NULL, 0)) == (char *)(-1))
	{
		perror("shmat error");
		exit(1);
	}
	int num;
	s = shm;
	char *lastData = malloc(SHMSZ);
	strcpy(lastData, s);
	char *end = malloc(SHMSZ);
	sprintf(end, "%d", 1000);

	// create semaphore
	sem_t *sem;
	sem = sem_open("/my_semaphore", O_CREAT, 0666, 1);
	if (sem == SEM_FAILED)
	{
		perror("sem_open error");
		exit(1);
	}

	while ((strcmp(end, s) != 0))
	{
		// wait for semaphore
		if (sem_wait(sem) == -1)
		{
			perror("sem_wait error");
			exit(1);
		}

		if ((strcmp(lastData, s) != 0))
		{
			sscanf(s,"%d", &num);
			sprintf(s, "%d , %d", num, num * 2);
			printf("Prod: %s\n", s);
			strcpy(lastData, s);
		}
		else
		{
			printf("Currently s is %s\n", s);
			//sleep(1);
		}

		// release semaphore
		if (sem_post(sem) == -1)
		{
			perror("sem_post error");
			exit(1);
		}
	}

	free(lastData);
	free(end);

	return 0;
}
