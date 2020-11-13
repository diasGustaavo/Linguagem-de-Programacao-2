#include <iostream>
#include <thread>
#include <chrono>
#include "random.hpp"

// Faça um programa em C/C++ que declare uma variável global do tipo double; faça o programa
// criar 10 threads e passe para essas threads um valor inteiro que represente um índice único (de 0 a 9). Faça
// cada uma das threads realizar uma função que verifique se seu identificador é par ou ímpar. Caso o
// identificador seja par, a função deve realizar um laço de repetição que faça 10 incrementos (+1) na variável
// global; caso o identificador seja ímpar, a função deve realizar um laço que faça 10 decrementos (-1) na
// variável global. A cada iteração a função deve exibir o identificador da thread e o novo valor da variável
// global, além de dormir um tempo aleatório entre 1s e 4s.
// Após o término de todas as threads, exiba o valor final da variável global na thread principal.

using Random = effolkronium::random_static;

double varGlobal = 0.0;
// std::mutex lock;

// funcao que as threads executarao
void funcaoThread(int indice)
{
    // verificando se indice eh par ou impar
    if(indice % 2 == 0){
        int i;
        for(i = 0; i < 10; i++){
            // tempo de dormir definido por numero aleatorio menor que 5
            int tempo = Random::get(1, 4);
            std::this_thread::sleep_for(std::chrono::seconds(tempo));
            // lock.lock()
            varGlobal++;
            // lock.unlock()
            std::cout << "Olá, eu sou a thread com identificador #" << indice << std::endl << "Alterei o valor de varGlobal para: " << varGlobal << std::endl;
        }
    }
    else{
        int i;
        for(i = 0; i < 10; i++){
            // tempo de dormir definido por numero aleatorio menor que 5
            int tempo = Random::get(1, 4);
            std::this_thread::sleep_for(std::chrono::seconds(tempo));
            // lock.lock()
            varGlobal--;
            // lock.unlock()
            std::cout << "Olá, eu sou a thread com identificador #" << indice << std::endl << "Alterei o valor de varGlobal para: " << varGlobal << std::endl;
        }
    }
    
    // encerrando thread
}

int main(int argc, char *argv[])
{
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

    std::vector <std::thread> threads;

    for(int t = 0; t < 10; ++t){
        std::thread threadObj(funcaoThread, std::ref(t));
        threads.push_back(std::move(threadObj));
    }

    // main aguarda todas as threads encerrarem
	for(auto &threadObj: threads){
		threadObj.join();
	}

    // note que o valor da var global pode nao ser a esperada, dado que os proce-
    // ssos concorrentes acessando a mesma posicao de memoria podem ter corrompi-
    // do-a
    std::cout << "Valor de var global: " << varGlobal << std::endl;

    return 0;
}
