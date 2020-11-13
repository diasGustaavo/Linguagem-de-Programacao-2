#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Faça um programa em C/C++ em que você compare o tempo de criação de 30 threads e 30
// processos, onde as threads e processos criadas apenas executem uma função que exiba uma mensagem
// ("Nova thread" ou "Novo processo") e as faça dormir por 2s.
// Use alguma função de medição de tempo* para tal e escreva suas impressões sobre o resultado, justificando
// qual das opções demandou menos tempo de execução para ser realizada e faça a relação disso com consumo
// de processamento e memória. Inclua junto ao código um print de uma execução do programa. Para finalizar,
// faça uma clara distinção (em comentários no código) das diferenças principais entre threads e processos. Ao
// término do programa, nenhum processo deverá permanecer em execução.

// A partir deste codigo, podemos observar a clara diferenca entre processos e threads
// Processos sao unidades independentes, com sua propria memoria independente
// Threads sao dependentes dos processos e compartilham memoria por natureza
// O resultado disso sao processos muito mais modulares porem mais pesados e 
// consequentemente devagarosos. Enquanto os Threads sao mais rapidos e leves.

// funcao que as threasd executarao
void* funcaoThread() {
    printf("Nova thread\n");
    // dormir por 2 segundos
    sleep(2);
    // encerrar thread
    pthread_exit(NULL) ;
}

// funcao que os processos executarao
void funcaoProcesso(int pidpai) {
    printf("Novo processo\n");
    // dormir por 2 segundos
    sleep(2);
    // se o PID nao for igual a pidpai, encerra o processo
    if(getpid() != pidpai)
        exit(0);
}

int main(void) {

    // --------------------------------------------------------
    // 
    //                      PROCESSOS
    //
    // _________________________________________________________

    // inicializando relogio para contar tempo de execucao
    clock_t begin = clock();

    // definindo pidpai
    int pidpai;
    pidpai = getpid();

    // criando processos
    for(int i = 0; i < 30; i++){
        // se PID for pidpai, cria novo processo
        if(getpid()==pidpai){
            // criando processo
            fork();
        }else{
            break;
        }
    }

    // processos executando a funcao concorrentemente
    funcaoProcesso(pidpai);

    // inicializa relogio final para contar tempo de execucao
    clock_t end = clock();
    // definindo tempo de execucao como a diferenca entre o 
    // tempo que comecou e o tempo que terminou
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo gasto com os processos: %lf\n", time_spent);


    // --------------------------------------------------------
    // 
    //                         THREADS
    //
    // _________________________________________________________

    // redefinido tempo de inicio
    begin = clock();

    // declarando as 30 threads
    pthread_t thread[30];
    int i;
    // criando as 30 threasd de fato, fazendo-as executar 'funcaoThread'
    for(i = 0; i < 30; ++i){
        pthread_create(&thread[i], NULL, funcaoThread, NULL);
    }

    // redefinindo tempo de termino
    end = clock();
    // redefinindo tempo de execucao como a diferenca entre o 
    // tempo que comecou e o tempo que terminou 
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo gasto com as threads: %lf\n", time_spent);

    // podemos observar que, como esperado, threads sao mais rapidas, consumindo menos
    // tempo, processamento e memoria. Entretanto, processos sao mais modulares

    return 0;
}