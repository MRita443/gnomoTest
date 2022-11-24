#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{

	int numberProcesses = atoi(argv[1]);
	float blockProb = atof(argv[2]);
	unsigned sleepTime = atoi(argv[3]);
	int token = 0;
	pid_t pid;
	int fileDescriptor;

	for (int i = 0; i < numberProcesses; i++)
	{
		char pipeName[] = fstring("pipe%dto%d", i + 1, i + 2);
		if (mkfifo(pipeName, 0666) != 0)
		{
			printf("Unable to create a fifo; errno=%d\n", errno);
			return EXIT_FAILURE;
		}
	}

	char lastPipeName[] = fstring("pipe%dto%d", numberProcesses, 1);
	if (mkfifo(lastPipeName, 0666) != 0)
	{
		printf("Unable to create a fifo; errno=%d\n", errno);
		return EXIT_FAILURE;
	}

	if ((pid = fork()) < 0)
	{
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
	}
	else if (pid == 0)
	{
		
	}
	return EXIT_SUCCESS;
}
