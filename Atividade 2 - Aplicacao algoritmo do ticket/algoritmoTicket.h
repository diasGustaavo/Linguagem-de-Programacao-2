#ifndef _ALGORITMO_TICKET_C_
#define _ALGORITMO_TICKET_C_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

typedef struct s_ticket{
    unsigned int ticketAtual;
    unsigned int proxTicketGerado;
}ticket;

typedef struct s_infoGeralParque infoGeralParque;

//struct para encapsular os argumentos
typedef struct s_argsThread{
    int identificador;
    infoGeralParque* infoParque;
}argsThread;

typedef struct s_argsThreadCarro{
    unsigned int pessoasNoCarro;
    bool* voltaAcabou;
    infoGeralParque* infoParque;
}argsThreadCarro;

typedef struct s_infoGeralParque{
    int* numDeVoltas;
    int* pessoasNaFila;
    bool* parqueAberto;
    ticket* maquinaDeTicket;
    argsThreadCarro* infoCarro;
}infoGeralParque;

unsigned int ticket_acquire(ticket *lock);
void ticket_init(ticket *lock);
void ticket_release(ticket *lock);
void* threadPessoa(void *_parametros);
void* funcaoCarro(void *_parametros);

#endif