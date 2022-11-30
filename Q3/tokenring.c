#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

char **pipes;

int child(int processID, int blockProb, int sleepTime)
{
	
	int token = -1;
	srand(time(NULL) ^ getpid());
	int fd = open(pipes[processID - 1], O_RDONLY);
	int fw = open(pipes[processID], O_WRONLY);
	while (1)
	{
		if(read(fd, &token, sizeof(int)) == -1){
			printf("p%d error while reading\n", processID);
		}
		int probability = rand() % 100;
		if (probability <= blockProb*100)
		{
			printf("[p%d] lock on token (val = %d)\n", processID, token);
			sleep(sleepTime);
			printf("[p%d] unlock token\n", processID);
		}
		token++;
		write(fw, &token, sizeof(int));
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL) ^ getpid());
	int numberProcesses = atoi(argv[1]) + 1; //lmao
	pipes = (char **)malloc(sizeof(char **) * numberProcesses);
	int blockProb = atof(argv[2]);
	unsigned sleepTime = atoi(argv[3]);
	int token = 0;
	pid_t pid;

	for (int i = 0; i < numberProcesses - 1; i++)
	{
		char * pipeName = (char *) malloc(20);
		sprintf(pipeName, "pipe%dto%d", i + 1, i + 2);

		if (mkfifo(pipeName, 0666) == -1)
		{
			printf("Unable to create a fifo; errno=%d\n", errno);
			return EXIT_FAILURE;
		}
		else{
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
	for (int i = 2; i <= numberProcesses; i++)
	{
		if ((pid = fork()) < 0)
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			child(i-1, blockProb, sleepTime);
		}
	}
	int fw = open(pipes[0], O_WRONLY);
	write(fw, &token, sizeof(int));
	int fd = open(pipes[numberProcesses - 1], O_RDONLY);
	while (1)
	{
		read(fd, &token, sizeof(int));
		int probability = rand() % 100;
		if (probability <= blockProb*100)
		{
			printf("[p%d] lock on token (val = %d)\n", 1, token);
			sleep(sleepTime);
			printf("[p%d] unlock token\n", 1);
		}
		token++;
		write(fw, &token, sizeof(int));
	}
	return EXIT_SUCCESS;
}
