#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    for (int i = 1; i <= argc; i++)
    {
        if ((pid = fork()) < 0)
        {
            perror("fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            char buffer[255];
            char *test = strtok(argv[i], ".");
            sprintf(buffer, "pandoc %s.txt -o %s.epub", argv[i], test);
            system(buffer);
            printf("[%d] converting %s", getppid(), argv[i]);
        }
        else if (pid > 0)
        {
            wait();
            char *args;
            char buffer[255];
            for (int i = 1; i <= argc; i++)
            {
                char* clean = strtok(argv[i], ".");
                strcat(args, strcat(clean, ".epub"));
            }
            sprintf(buffer, "zip ebooks.zip ", args);
            system(buffer);
        }
    }
    return EXIT_SUCCESS;
}