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
    fseek(f,0,SEEK_END);
    long size = ftell(f);
    fseek(f,0,SEEK_SET);
    return size;
}

int main(int argc, char* argv[]){
    FILE* f = fopen(argv[1],"r");
    int m = atoi(argv[2]);
    size_t n = atoi(argv[3]);
    if (f){
        srandom(0);  
        long size = getSize(f);
        for (int i = 0; i<m; i++){          
            int pos = (random()%(size-n));
            
            fseek (f, pos, SEEK_SET);
            char* buffer = malloc(n);
            fread(buffer,1,n,f);
            printf(">%s<\n",buffer);
        }
        fclose(f);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}