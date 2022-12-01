#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
argv[1] -> texto
argv[2] -> número de fragmentos
argv[3] -> tamanho dos fragmentos 
*/

long getSize(FILE* f){
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

int main(int argc, char* argv[]){
    if (argc != 4){
        printf("Insert the correct number of arguments. samples <file name> <number of fragments> <size per fragment>\n");
        return EXIT_SUCCESS;
    }
    FILE* f = fopen(argv[1], "r");
    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    if (m <= 0){
        printf("Invalid number of fragments\n");
        return EXIT_SUCCESS;
    }
    if (n <= 0){
        printf("Invalid size of fragments\n");
        return EXIT_SUCCESS;
    }
    if (f){
        srandom(0);  
        long size = getSize(f);
        for (int i = 0; i < m; i++){          
            int pos = (random() % (size - n));           
            fseek(f, pos, SEEK_SET);
            char* buffer = malloc(n);
            fread(buffer, 1, n, f);
            printf(">%s<\n", buffer);
        }
        fclose(f);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}