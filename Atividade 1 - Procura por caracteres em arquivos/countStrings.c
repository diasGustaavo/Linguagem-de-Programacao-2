#ifndef COUNT_STRINGS_H
#define COUNT_STRINGS_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
    char stringDesejada[11];
    strcpy(stringDesejada, argv[1]);
    int tamanhoRealString = strlen(stringDesejada);
    FILE *fp;
    char str[10];
    char filename[10];
    strcpy(filename, argv[2]);
    strcat(filename, ".txt");

    fp = fopen(filename, "r");
    if (fp == NULL){
        return 1;
    }

    int counterWords = 0;
    while (fgets(str, 10, fp) != NULL){
        int index = 0;
        int counterLetters = 0;
        while(index < 10 ){
            if(str[index] == stringDesejada[counterLetters]){
                counterLetters++;
                if(counterLetters >= tamanhoRealString){
                    counterWords++;
                    counterLetters = 0;
                }
            }
            else{
                counterLetters = 0;
            }

            ++index;
            }
    }

    fclose(fp);

    exit(counterWords);
}

#endif