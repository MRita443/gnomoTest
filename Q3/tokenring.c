#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

	int numberProcesses = atoi(argv[1]);
	float blockProb = atof(argv[2]);
	unsigned sleepTime = atoi(argv[3]);
	char *pipes[numberProcesses];
	int token = 0;
	int currentPipe = 0;
	pid_t pid;
	int fd;
	int fileDescriptor;

	for (int i = 0; i < numberProcesses - 1; i++){
		char* pipeName;
		sprintf(pipeName, "pipe%dto%d", i + 1, i + 2);

		if (mkfifo(pipeName, 0666) != 0){
			printf("Unable to create a fifo; errno=%d\n", errno);
			return EXIT_FAILURE;
		}
		else{
			pipes[i] = pipeName;
		}
	}

	char* lastPipeName;
	sprintf("pipe%dto%d", numberProcesses, 1);

	if (mkfifo(lastPipeName, 0666) != 0){
		printf("Unable to create a fifo; errno=%d\n", errno);
		return EXIT_FAILURE;
	}
	else{
		pipes[numberProcesses - 1] = lastPipeName; //Adiciona o último pipe criado à lista de pipes
	}

	if ((pid = fork()) < 0){
		perror("fork error");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0){
		fd = open(pipes[currentPipe], O_WRONLY);
		write(fd, token, sizeof(int));
		close(fd);
	}
	else if (pid == 0){
		fd = open(pipes[currentPipe++], O_RDONLY);
		currentPipe %= numberProcesses; //No caso, de o já ter sido utilizado o último pipe, voltar ao primeiro
		read(fd, token, sizeof(int));
		printf(token);
		close(fd);
	}
	return EXIT_SUCCESS;
}
