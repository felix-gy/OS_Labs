#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#define SHMSZ 10

int main()
{
	char *shm;
	char *s;
	int shmid;
	key_t key;
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
	s = shm;
	char *lastData = malloc(SHMSZ);
	strcpy(lastData, s);
	int seq = 100;
	char *end = malloc(SHMSZ);
	sprintf(end, "%d", 1000);

	// Crear semáforo
	sem_t *sem;
	sem = sem_open("/mysemaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
	if (sem == SEM_FAILED)
	{
		perror("sem_open error");
		exit(1);
	}

	while ((strcmp(end, s) != 0))
	{
		// Bloquear el semáforo
		if (sem_wait(sem) != 0)
		{
			perror("sem_wait error");
			exit(1);
		}

		if ((strcmp(lastData, s)) != 0)
		{
			sprintf(s, "%d", seq);
			printf("Detect change Gen, change to ->  %s\n", s);
			strcpy(lastData, s);
			seq += 20;
		}
		else
		{
			printf("Without change is -> %s\n", s);
			//sleep(1);
		}

		// Desbloquear el semáforo
		if (sem_post(sem) != 0)
		{
			perror("sem_post error");
			exit(1);
		}
	}

	free(lastData);
	free(end);

	// Cerrar y eliminar el semáforo
	if (sem_close(sem) != 0)
	{
		perror("sem_close error");
		exit(1);
	}
	if (sem_unlink("/mysemaphore") != 0)
	{
		perror("sem_unlink error");
		exit(1);
	}

	return 0;
}