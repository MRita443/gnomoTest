#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
argv[1] -> Texto
argv[2] -> Número de fragmentos
argv[3] -> Tamanho dos fragmentos
*/

long getSize(FILE *f)
{
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

int main(int argc, char *argv[])
{
    if (argc != 4) // Confirma que o número de argumentos passados é o correcto, ou seja, se foi chamada a função e os seus 3 parâmetros 
    {
        printf("Insert the correct number of arguments. samples <file name> <number of fragments> <size per fragment>\n");
        return EXIT_FAILURE;
    }
    FILE *f = fopen(argv[1], "r"); // Abre o ficheiro para ser lido
    if (!f)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    if (m <= 0)
    {
        printf("Invalid number of fragments\n");
        return EXIT_FAILURE;
    }
    if (n <= 0)
    {
        printf("Invalid size of fragments\n");
        return EXIT_FAILURE;
    }

    srandom(0); // Cria uma seed para ir buscar pseudo-aleatoriamente as posições dos fragmentos
    long size = getSize(f);
    for (int i = 0; i < m; i++)
    {
        int pos = (random() % (size - n)); // Gera uma posição inicial com garantia que está a pelo menos n caracteres do fim do ficheiro cada vez que se quer criar um fragmento
        fseek(f, pos, SEEK_SET);
        char *buffer = malloc(n); // Cria um pointer chamado buffer para uma memória alocada de tamanho n cada vez que se quer criar um fragmento
        fread(buffer, 1, n, f);
        printf(">%s<\n", buffer); // Imprime o fragmento
    }
    if (fclose(f) != 0)
    {
        perror("Error closing file");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
