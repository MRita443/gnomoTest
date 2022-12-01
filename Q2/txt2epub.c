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

    for (int i = 1; i < argc; i++) // O ciclo repete-se para cada ficheiro .txt
    {
        char *argumentCopy = (char *)malloc(strlen(argv[i]) + 1);
        strcpy(argumentCopy, argv[i]); // Copia o ficheiro .txt para uma variável temporária

        char *fileNameTxt = argv[i];
        char *fileNameEpub = strtok(argumentCopy, "."); // Quebra a variável temporária numa série de tokens usando o "." como delimitador
        strcat(fileNameEpub, ".epub"); // Adiciona .epub

        if ((pid = fork()) < 0) // Dá erro se a criação do processo não for bem sucedida
        {
            perror("Fork error");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Converte um ficheiro .txt em .epub usando o pandoc
        {
            char *pandocArgs[] = {"pandoc", fileNameTxt, "-o", fileNameEpub, NULL};
            printf("[%d] converting %s\n", getppid(), fileNameTxt);
            execvp("pandoc", pandocArgs);
        }
        else if (pid > 0) // Adiciona o ficheiro .epub
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
