#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// Faça um programa em C/C++ onde você crie 10 threads, e estas threads vão representar lebres em
// uma corrida fictícia. Faça as threads executarem uma função que receba dois parâmetros: um valor inteiro
// que identifique a thread (de 0 a 9) e um valor real (do tipo float ou double) que represente a distância
// máxima do salto da lebre (um valor aleatório gerado entre 20 e 40, a unidade considerada será cm).
// Faça as lebres "correrem" em uma "pista" com 10m, ou seja, a função de cada thread (lebre) deverá
// executar a geração de valores aleatórios (entre 0 e a distância máxima do salto da lebre) e a soma destes em
// uma variável que represente o quanto a lebre já percorreu na pista (uma variável real no escopo da função
// que a thread executa). A cada "salto" (iteração realizada pela função), deverá ser exibida uma mensagem
// contendo a distância do salto efetuado e qual distância total percorrida (dormindo 1s - função sleep - até que
// dê outro salto). A primeira thread (lebre) cuja variável que represente o quanto a lebre já percorreu na pista
// atinja o valor de 1000cm

//struct para encapsular os argumentos
typedef struct s_argsThread{
    int identificador;
    float distanciaMax;
}argsThread;

//retorna uma distancia aleatoria entre 0.4 e 0.2
float gerarDistanciaMax(){
    float dist = 0.0;
    while(dist >= 0.4 || dist <= 0.2){
        dist = ((float)rand() / 4000000000.0);
    }
    return dist;
}

//retorna uma distancia aleatoria ate distanciaMax
float gerarDistanciaPulo(float distanciaMax){
    float dist = 9999.0;
    while(dist > distanciaMax){
        dist = ((float)rand() / 40000000.0);
    }
    return dist;
}

// funcao que as threads executarao
void* funcaoThread(void *_parametros) {
    //transformando de void para argsThread
    argsThread *parametros = _parametros;
    //definindo distPercorrida
    float distPercorrida = 0.0;
    //executar o loop enquanto a distancia percorrida for menor que 10.0
    while(distPercorrida < 10.0){
        //para cada pulo, gera uma distancia diferente
        float distPulo = gerarDistanciaPulo(parametros->distanciaMax);
        // soma a distancia percorrida
        distPercorrida += distPulo;
        // printa o status e dorme por 1 segundo
        printf("Lebre #%d\nDistancia do salto: %f\nDistancia total: %f\n\n\n", parametros->identificador, distPulo, distPercorrida);
        sleep(1);
    }

    // quem atingir aqui primeiro venceu a corrida
    // encerrando o programa
    printf("Eu sou a lebre numero %d e eu venci esta corrida!\n", parametros->identificador);
    exit(0);
}

int main() {
    //inicializando o contador i
    int i;
    //inicializando as threads e seus argumentos
    pthread_t thread[10];
    argsThread argumentos[10];
    //usando time como seed para gerar numero aleatorio
    srand(time(0));
    //cria de fato todas as threads com os seus respectivos argumentos
    for(i = 0; i < 10; ++i){
        argumentos[i].distanciaMax = gerarDistanciaMax();
        argumentos[i].identificador = i;
        pthread_create(&thread[i], NULL, funcaoThread, &argumentos[i]);
    }
    
    //espera o termino da execucao de cada thread
    for(int i = 0; i<10; i++){
      pthread_join(thread[i], NULL);
    }
}