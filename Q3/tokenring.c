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
	if (argc != 4)
	{
		printf("Insert the correct number of arguments. tokenring <number of processes> <blocking probability> <block time>\n");
		return EXIT_FAILURE;
	}

	srand(time(NULL));

	int numberProcesses = atoi(argv[1]);
	int blockProb = atof(argv[2]);
	unsigned sleepTime = atoi(argv[3]);

	pipes = (char **)malloc(sizeof(char *) * numberProcesses);

	int token = 0;
	pid_t pid;

	for (int i = 0; i < numberProcesses; i++)
	{
		char *pipeName = (char *)malloc(20);
		sprintf(pipeName, "pipe%dto%d", i + 1, 1 + (i + 1) % numberProcesses);

		if (mkfifo(pipeName, 0666) == -1)
		{
			perror("Unable to create a FIFO");
			return EXIT_FAILURE;
		}
		else
		{
			pipes[i] = pipeName;
		}
	}

	for (int i = 1; i < numberProcesses; i++)
	{
		if ((pid = fork()) < 0)
		{
			perror("Fork error");
			return EXIT_FAILURE;
		}
		else if (pid == 0)
		{
			int fd = open(pipes[i - 1], O_RDONLY);
			if (fd < 0)
			{
				perror("Error opening pipe");
				return EXIT_FAILURE;
			}

			int fw = open(pipes[i], O_WRONLY);
			if (fw < 0)
			{
				perror("Error opening pipe");
				return EXIT_FAILURE;
			}

			while (1)
			{
				if ((read(fd, &token, sizeof(int)) != sizeof(token)))
				{
					perror("Read error");
					return EXIT_FAILURE;
				}
				int probability = rand() % 100;
				if (probability <= blockProb * 100)
				{
					printf("[p%d] Pid:%d lock on token (val = %d)\n", i + 1, getpid(), token);
					sleep(sleepTime);
					printf("[p%d] Pid:%d unlock token\n", i + 1, getpid());
				}
				token++;
				if ((write(fw, &token, sizeof(int))) != sizeof(token))
				{
					perror("Write error");
					return EXIT_FAILURE;
				}
			}
		}
		printf("Created child %d\n", pid);
	}
	int fw = open(pipes[0], O_WRONLY);
	if (fw < 0)
	{
		perror("Error opening pipe");
		return EXIT_FAILURE;
	}
	int fd = open(pipes[numberProcesses - 1], O_RDONLY);
	if (fd < 0)
	{
		perror("Error opening pipe");
		return EXIT_FAILURE;
	}
	while (1)
	{
		int probability = rand() % 100;
		if (probability <= blockProb * 100)
		{
			printf("[p%d] Pid:%d lock on token (val = %d)\n", 1, getpid(), token);
			sleep(sleepTime);
			printf("[p%d] Pid:%d unlock token\n", 1, getpid());
		}
		token++;
		if ((write(fw, &token, sizeof(int))) != sizeof(token))
		{
			perror("Write error");
			return EXIT_FAILURE;
		}
		if ((read(fd, &token, sizeof(int)) != sizeof(token)))
		{
			perror("Read error");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
