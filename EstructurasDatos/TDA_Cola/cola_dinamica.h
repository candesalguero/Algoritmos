#ifndef COLA_DINAMICA_H_INCLUDED
#define COLA_DINAMICA_H_INCLUDED

#define COLA_LLENA -5
#define COLA_VACIA -6

#include "../../config.h"

typedef struct sNodoC
{
    void *info;
    size_t tamElem;
    struct sNodoC *sig, *ant;
}tNodoC;

typedef struct
{
    tNodoC *pri;
    tNodoC *ult;
}tCola;

void crearCola(tCola *p);
int colaLlena(const tCola *p, unsigned cantBytes);
int ponerEnCola(tCola *p, const void *elem, unsigned cantBytes);
int verPrimeroCola(const tCola *p, void *elem, unsigned cantBytes);
int colaVacia(const tCola *p);
int sacarDeCola(tCola *p, void *elem, unsigned cantBytes);
void vaciarCola(tCola *p);

#endif /*/ COLA_DINAMICA_H_INCLUDED*/
