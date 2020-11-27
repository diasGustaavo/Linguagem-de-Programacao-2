// Suponha que existam 20 pessoas e um carro de uma montanha russa em um parque. As pessoas,
// repetidamente, esperam para dar uma volta no carro. O carro tem capacidade para 5 pessoas. Antes de começar
// uma volta, o carro deve aguardar um tempo fixo e partir, mesmo que não esteja cheio. Após dar uma volta na
// montanha russa (volta com tempo aleatório), cada pessoa passeia pelo parque de diversões e depois retorna à
// montanha russa para a próxima volta. O programa a ser desenvolvido deverá utilizar o algoritmo do ticket
// apresentado em sala de aula.

// Tanto o carro quanto as pessoas devem ser representados por threads. O programa deverá ser finalizado
// (parque fechar) depois que o carro der 10 voltas. A implementação das threads das pessoas devem basear-se no
// seguinte pseudocódigo:

// thread pessoa[i = 1 to 20] {
//     while (!fechouParque) {
//         entraNoCarro();
//         /* Incrementa contador que registra o passageiros transportados pelo carro. */
//         esperaVoltaAcabar();
//         saiDoCarro(); // decrementa o contador
//         passeiaPeloParque(); // tempo aleatório
//     }
// }

// Da mesma forma, a implementação da thread do carro deverá a seguinte estrutura:

// thread carro {
//     while (existemPassageirosNoParque) {
//         esperaTempoArbitrario();
//         daUmaVolta(); //Tempo aleatório
//         esperaEsvaziar();
//         volta++; // Indicador para o fechamento do parque.
//     }
// }

// A implementação deverá ser feita em C/C++ utilizando a biblioteca Pthreads ou as threads STD (C++11), e
// o algoritmo do ticket deverá ser implementado usando a função atômica fetch-and-add 1 . A implementação deverá
// atender às quatro propriedades de uma solução para o problema da seção crítica: exclusão mútua, ausência de
// deadlock, ausência de atraso desnecessário e entrada eventual. A saída do seu programa deve ser bem planejada,
// de forma a mostrar o que está acontecendo a cada momento.

#include "algoritmoTicket.h"

// funcao para adquirir um novo ticket
unsigned int ticket_acquire(ticket *lock) {
    return (__sync_fetch_and_add(&(lock->proxTicketGerado), 1));
}

// funcao para progredir para o proximo ticket
void ticket_release(ticket *lock) {
    lock->ticketAtual += 1;
}

// funcao que as threads das pessoas executarao
void* threadPessoa(void *_parametros) {
    // convertendo os argumentos para um tipo utilizavel
    argsThread* parametro = ((argsThread*)_parametros);
    // enquanto o parque estiver aberto as pessoas seguirao o ciclo
    while(*(parametro->infoParque->parqueAberto) == true)
    {
        // adquire um novo ticket
        unsigned int ticket = ticket_acquire(parametro->infoParque->maquinaDeTicket);
        // aponta se a pessoa passou pela fila. a primeira pessoa nao passara
        bool passouNaFila = false;
        int i = 0;
        // enquanto o ticket atual a ser atendido nao for igual ao ticket da thread
        while(parametro->infoParque->maquinaDeTicket->ticketAtual != ticket){
            // na primeira vez que a thread rodar o if, vai adicionar 1 ao
            // contador de pessoas na fila, vai alterar passouNaFila e 
            // vai informar sua id, ticket e o ticket que vai ser chamado
            if(i == 0){
                i++;
                passouNaFila = true;
                __sync_fetch_and_add(parametro->infoParque->pessoasNaFila, 1);
                printf("[PESSOA] id #%d com ticket %d aguardando, ticketAtual = %d\n", parametro->identificador, ticket, parametro->infoParque->maquinaDeTicket->ticketAtual);
            }
        }

        // entraNoCarro();
        // adiciona a quantidade de pessoas no carro
        __sync_fetch_and_add(&(parametro->infoParque->infoCarro->pessoasNoCarro), 1);
        // se estava na fila, ao sair vai reduzir o numero de pessoas na fila
        if(passouNaFila == true)
            __sync_fetch_and_sub(parametro->infoParque->pessoasNaFila, 1);
        // se ela nao for a quinta (ultima) pessoa a entrar no carro,
        // ela aciona o mecanismo para avancar a maquinaDeTicket
        if(parametro->infoParque->infoCarro->pessoasNoCarro < 5)
            ticket_release(parametro->infoParque->maquinaDeTicket);
        printf("[PESSOA] id #%d entrou no carro\n", parametro->identificador);

        // esperaVoltaAcabar();
        while(*(parametro->infoParque->infoCarro->voltaAcabou) != true){
        }

        // saiDoCarro();
        __sync_fetch_and_sub(&(parametro->infoParque->infoCarro->pessoasNoCarro), 1);

        // passeiaPeloParque();
        int tempo = rand() % 10;
        sleep(tempo);
    }
    exit(0);
}

void* funcaoCarro(void *_parametros) {
    // convertendo os argumentos para um tipo utilizavel
    argsThreadCarro* parametro = ((argsThreadCarro*)_parametros);
    // repete enquanto houver pessoas na fila e enquanto o numero de voltas for inferior a 10
    while(parametro->infoParque->pessoasNaFila > 0 && *(parametro->infoParque->numDeVoltas) < 10){
        // restaura a variavel voltaAcabau para false
        *(parametro->voltaAcabou) = false;
        // avanca a maquina de ticket para o primeiro passageiro entrar
        ticket_release(parametro->infoParque->maquinaDeTicket);

        // esperaTempoArbitrario();
        // espera os passageiros entrarem por 'tempo' segundos
        int tempo = rand() % 10;
        printf("[CARRO] esperando %d segundos...\n", tempo);
        sleep(tempo);

        // daUmaVolta();
        // da uma volta de 'tempoVolta' segundos
        int tempoVolta = rand() % 10;
        printf("[CARRO] fazendo uma volta de %d segundos com %d pessoas...\n", tempoVolta, parametro->pessoasNoCarro);
        sleep(tempoVolta);

        // define que a volta acabou
        *(parametro->voltaAcabou) = true;

        // esperaEsvaziar();
        // espera as pessoas sairem do carro
        int i = 0;
        while(parametro->pessoasNoCarro != 0)
        {   
            if(i == 0){
                i++;
                printf("[CARRO] esperando esvaziar\n");
            }
        }

        // volta++;
        // a volta acabou, logo aumenta o contador de voltas
        *(parametro->infoParque->numDeVoltas) += 1;
        printf("[CARRO] NUM DE VOLTAS: %d\n", *(parametro->infoParque->numDeVoltas));
    }

    bool statusParque = false;
    parametro->infoParque->parqueAberto = &(statusParque);
    exit(0);
}

int main() {
    //inicalizando gerador de numeros aleatorios com o horario atual como seed
    srand(time(0));
    //declarando os parametros para as pessoas e para o carro
    argsThread parametros[20];
    argsThreadCarro parametrosCarro;
    //declarando as informacoes gerais do parque
    int numDeVoltas, pessoasNaFila = 0;
    bool parqueAbertoLocal = true;
    bool voltaAcabouLocal = false;
    ticket maquinaTicketGeral = {-1, 0};
    infoGeralParque infoParque = {&numDeVoltas, &pessoasNaFila, &parqueAbertoLocal, &maquinaTicketGeral, &parametrosCarro};

    //declarando uma thread do carro e ajustando os seus parametros
    pthread_t threadCarro;
    parametrosCarro.pessoasNoCarro = 0;
    parametrosCarro.voltaAcabou = &voltaAcabouLocal;
    parametrosCarro.infoParque = &infoParque;
    pthread_create(&threadCarro, NULL, funcaoCarro, &parametrosCarro);
    
    //declarando as threads das pessoas e ajustando os seus parametros
    pthread_t thread[20];
    int t;
    for (t = 0; t < 20; t++) {
        parametros[t].identificador = t;
        parametros[t].infoParque = &infoParque;
        pthread_create(&thread[t], NULL, threadPessoa, &parametros[t]);
    }

    //esperando a threadCarro finalizar...
    pthread_join(threadCarro, NULL);

    //esperando todas as threads das pessoas acabarem
    int i;
    for(i = 0; i<20; i++){
        pthread_join(thread[i], NULL);
    }

    return 0;
}