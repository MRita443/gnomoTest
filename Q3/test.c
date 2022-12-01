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

int main(int argc, char *argv[])
{
	srand(time(NULL));

	int numberProcesses = atoi(argv[1]);
	float blockProb = atof(argv[2]);
	unsigned sleepTime = atoi(argv[3]);

	pipes = (char **)malloc(sizeof(char *) * numberProcesses);

	int token = 0;
	pid_t pid;

	// char *pipeName = (char *)malloc(20);
	char pipeName[20] = {'\0'};

	for (int i = 0; i < numberProcesses; i++)
	{

		sprintf(pipeName, "pipe%dto%d", i + 1, 1 + (i + 1) % numberProcesses);

		if (mkfifo(pipeName, 0666) == -1)
		{
			printf("Unable to create a fifo; errno=%d\n", errno);
			return EXIT_FAILURE;
		}
		else
		{
			pipes[i] = pipeName;
			printf("%s\n", pipes[i]);
		}
	}

	int fd, fw;
	for (int i = 1; i < numberProcesses; i++)
	{
		if ((pid = fork()) < 0)
		{
			perror("fork error");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			printf("i: %d\n", i);

			while (1)
			{
				fd = open(pipes[i - 1], O_RDONLY);
				printf("p%d reading from %s\n", i + 1, pipes[i - 1]);
				if (read(fd, &token, sizeof(int)) == -1)
				{
					printf("p%d error while reading\n", i + 1);
				}
				close(fd);
				token++;
				int probability = rand() % 100;
				if (probability <= blockProb * 100)
				{
					printf("[p%d] lock on token (val = %d)\n", i + 1, token);
					sleep(sleepTime);
					printf("[p%d] unlock token\n", i + 1);
				}
				// token++;
				fw = open(pipes[i], O_WRONLY);
				printf("p%d writing to %s\n", i + 1, pipes[i]);
				write(fw, &token, sizeof(int));
				close(fw);
			}
		}
	}

	fw = open(pipes[0], O_WRONLY);
	printf("p%d writing to %s\n", 1, pipes[0]);
	write(fw, &token, sizeof(int));


	int i = 0;
	token = 0;
	while (1)
	{
		fd = open(pipes[numberProcesses - 1], O_RDONLY);
		printf("p%d reading from %s\n", 1, pipes[numberProcesses - 1]);
		read(fd, &token, sizeof(int));
		close(fd);
		int probability = rand() % 100;
		if (probability <= blockProb * 100)
		{
			printf("[p%d] lock on token (val = %d)\n", i + 1, token);
			sleep(sleepTime);
			printf("[p%d] unlock token\n", i + 1);
		}
		token++;
		//  escreve;
		if ((write(fw, &token, sizeof(int))) != sizeof(token))
			perror("write");
		close(fw);
		// token++;
		// write(fw, &token, sizeof(int));
	}
	return EXIT_SUCCESS;
}