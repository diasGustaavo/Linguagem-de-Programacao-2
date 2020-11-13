#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Faça um programa em C/C++ que declare uma variável global do tipo double; faça o programa
// criar 10 threads e passe para essas threads um valor inteiro que represente um índice único (de 0 a 9). Faça
// cada uma das threads realizar uma função que verifique se seu identificador é par ou ímpar. Caso o
// identificador seja par, a função deve realizar um laço de repetição que faça 10 incrementos (+1) na variável
// global; caso o identificador seja ímpar, a função deve realizar um laço que faça 10 decrementos (-1) na
// variável global. A cada iteração a função deve exibir o identificador da thread e o novo valor da variável
// global, além de dormir um tempo aleatório entre 1s e 4s.
// Após o término de todas as threads, exiba o valor final da variável global na thread principal.

double varGlobal = 0.0;
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// funcao que as threads executarao
void *funcaoThread(void *indiceThread)
{
    // inicializando o gerador de numeros aleatorios com o tempo como seed
    srand(time(0));
    // resgatando o indice passado nos parametros
    int indice;
    indice = (int)indiceThread;

    // verificando se indice eh par ou impar
    if(indice % 2 == 0){
        int i;
        for(i = 0; i < 10; i++){
            // tempo de dormir definido por numero aleatorio menor que 5
            int tempo = (rand() % 5);
            sleep(tempo);
            // pthread_mutex_lock(&lock);
            varGlobal++;
            // pthread_mutex_unlock(&lock);
            printf("Olá, eu sou a thread com identificador #%d!\nAlterei o valor de varGlobal para: %lf\n", indice, varGlobal);
        }
    }
    else{
        int i;
        for(i = 0; i < 10; i++){
            // tempo de dormir definido por numero aleatorio menor que 5
            int tempo = (rand() % 5);
            sleep(tempo);
            // pthread_mutex_lock(&lock);
            varGlobal--;
            // pthread_mutex_unlock(&lock);
            printf("Olá, eu sou a thread com identificador #%d!\nAlterei o valor de varGlobal para: %lf\n", indice, varGlobal);
        }
    }
    
    // encerrando thread
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    // declarando as threads
    pthread_t thread[10];

    // criando as threads efetivamente, utilizando o endereco da thread declarada
    // previamente, fazendo rodar a 'funcaoThread' usando como parametros 't'
    //
    // note que a criação de multiplas threads alterando uma unica variavel global
    // causa problemas, dado que duas threads ou maispodem alterar a variavel em um
    // mesmo instante, causando inconsistencia de memoria
    //
    // concorrencia é isso, dois ou mais processos (ou threads no caso) executando
    // em paralelo, acessando recursos compartilhados
    //
    // neste caso, poderiamos utilizar os mutex locks para bloquear o acesso a
    // variavel global enquanto outro processo estivesse usando-a.
    //
    // entretanto, o uso do mutex lock neste caso poderia fazer o programa ser
    // menos eficiente do que se fosse um programa sem uso de concorrencia
    // dado que os threads seriam 'congelados' e 'descongelados' multiplas
    // vezes para cooperar com o lock
    long t;
    for(t = 0; t < 10; ++t){
        pthread_create(&thread[t], NULL, funcaoThread, (void*)t);
    }

    // main aguarda todas as threads encerrarem
    for(t = 0; t < 10; ++t){
        pthread_join(thread[t], NULL);
    }

    // note que o valor da var global pode nao ser a esperada, dado que os proce-
    // ssos concorrentes acessando a mesma posicao de memoria podem ter corrompi-
    // do-a
    printf("Valor de var global: %lf", varGlobal);

    return 0;
}
