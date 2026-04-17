#ifndef PILA_DIN_H_INCLUDED
#define PILA_DIN_H_INCLUDED

#include "config.h"

typedef struct
{
    char pila[TAM_PILA];
    unsigned tope;
}t_Pila;

void crearPila (t_Pila* p);
int pilaLlena (const t_Pila* p, unsigned tam);
int pilaVacia (const t_Pila* p);
void vaciarPila (t_Pila* p);
int apilarPila(t_Pila* p, const void* dato, unsigned tam);
int desapilarPila(t_Pila* p, void* dato, unsigned tam_dato);
int verTope(const t_Pila* p, void* dato, unsigned tam_dato);

#endif // PILA_DIN_H_INCLUDED
