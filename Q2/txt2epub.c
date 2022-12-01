#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    char *args[argc + 1];
    args[0] = "zip";
    args[1] = "ebooks.zip";

    for (int i = 1; i < argc; i++)
    {
        char *argumentCopy = (char *)malloc(strlen(argv[i]) + 1);
        strcpy(argumentCopy, argv[i]);

        char *fileNameTxt = argv[i];
        char *fileNameEpub = strtok(argumentCopy, ".");
        strcat(fileNameEpub, ".epub"); // Add .epub

        if ((pid = fork()) < 0)
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            char *pandocArgs[] = {"pandoc", fileNameTxt, "-o", fileNameEpub, NULL};
            printf("[%d] converting %s\n", getppid(), fileNameTxt);
            execvp("pandoc", pandocArgs);
        }
        else if (pid > 0)
        {
            int status;
            if (wait(&status) < 0)
            {
                fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
            }
            args[i + 1] = fileNameEpub;
        }
    }

    if (pid > 0)
    {
        args[argc + 1] = "\0";
        // Exemplo de validação
        if (execvp("zip", args) == -1) { 
            perror("execvp"); 
            return EXIT_FAILURE;
        }
        execvp("zip", args);
    }
    exit(EXIT_SUCCESS);
}