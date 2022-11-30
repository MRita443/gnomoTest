#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int numberProcesses = atoi(argv[1]);
	int blockProb = atof(argv[2]) * 100;
	unsigned sleepTime = atoi(argv[3]);
	char *pipes[numberProcesses];
	int token = 0;
	int currentPipe = 0;
	pid_t pid;
	int fd;
	int fileDescriptor;

	for (int i = 0; i < numberProcesses - 1; i++)
	{
		char pipeName[25];
		sprintf(pipeName, "pipe%dto%d", i + 1, i + 2);

		if (mkfifo(pipeName, 0666) == -1)
		{
			printf("Unable to create a fifo; errno=%d\n", errno);
			return EXIT_FAILURE;
		}
		else
		{
			pipes[i] = pipeName;
		}
	}

	char lastPipeName[25];
	sprintf(lastPipeName, "pipe%dto%d", numberProcesses, 1);

	if (mkfifo(lastPipeName, 0666) == -1)
	{
		printf("Unable to create a fifo; errno=%d\n", errno);
		return EXIT_FAILURE;
	}
	else
	{
		pipes[numberProcesses - 1] = lastPipeName; // Adiciona o último pipe criado à lista de pipes
	}
	while (1)
	{
		if ((pid = fork()) < 0)
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid > 0)
		{
			fd = open(pipes[currentPipe], O_WRONLY);
			write(fd, token, sizeof(int));
			close(fd);

			int status;
			wait(&status);
		}
		else if (pid == 0)
		{
			fd = open(pipes[currentPipe], O_RDONLY);
			read(fd, token, sizeof(int));
			float probability = (rand() * blockProb) % 1;
			if (probability <= blockProb)
			{
				printf("[p%d] lock on token (val = %d)\n", (currentPipe + 2) % numberProcesses, token);
				sleep(sleepTime);
				printf("[p%d] unlock token\n", (currentPipe + 2) % numberProcesses);
			}
			close(fd);
			token++;
			currentPipe++;
			currentPipe %= numberProcesses; // No caso, de o já ter sido utilizado o último pipe, voltar ao primeiro
		}
	}
	return EXIT_SUCCESS;
}
