#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct s_argsThread{
    int identificador;
    char letra;
}argsThread;

pthread_barrier_t barreira;

void* getRandomChar(void *argumentos){
    // casting dos argumentos
    argsThread* actual_argumentos = (argsThread*)argumentos;
    
    // gera letra aleatoria
    int i;
    for(i = 0; i < 65; i++){
        i = rand() % 90;
    }
    char iEmChar = (char)i;

    // aplica a letra obtida na estrutura de argumentos
    actual_argumentos->letra = iEmChar;
    printf("Thread %d obteve a letra %c\n", actual_argumentos->identificador, actual_argumentos->letra);
    // a thread so saira quando todos passarem da barreira
    pthread_barrier_wait(&barreira);
}

int main(){
    // inicializa o gerador de numeros pseudoaleatorios
    srand( (unsigned)time(NULL) );

    // percorre por 10 vezes
    int contador = 0;
    while(contador < 10){
        printf("\n\n-------RODANDO PELA %d VEZ-------\n\n", contador + 1);
        // inicializando a barreira
        pthread_barrier_init(&barreira, NULL, 7);

        // inicializando as threads e seus argumentos
        pthread_t thread[6];
        argsThread argumentos[6];

        // criando de fato as threads com suas ids
        for(int i = 0; i < 6; i++){
            argumentos[i].identificador = i;
            pthread_create(&thread[i], NULL, getRandomChar, &argumentos[i]);
        }

        printf("main esperando na barreira\n");
        pthread_barrier_wait(&barreira);
        printf("barreira liberada\n");

        // compondo o retorno
        char retorno[6];
        for(int j = 0; j < 6; j++){
            retorno[j] = argumentos[j].letra;
        }

        printf("%s\n", retorno);

        // decide se retorno eh palindromo ou nao
        if(retorno[0] == retorno[5] && retorno[1] == retorno[4] && retorno[2] == retorno[3]){
            printf("e palindromo\n");
        }
        else{
            printf("nao e palindromo\n");
        }
        
        // destruindo a barreira
        pthread_barrier_destroy(&barreira);

        contador++;
    }
}