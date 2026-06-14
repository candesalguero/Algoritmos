#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MINIMO(x,y) ((x)<(y)?(x):(y))
typedef struct sNodoCola {
    void *info;
    unsigned tamInfo;
    struct sNodoCola *sig;
} tNodoCola;

typedef struct {
    tNodoCola *pri;
    tNodoCola *ult;
}tCola;

void crear_cola(tCola *pc);
int  poner_en_cola(tCola *pc, const void *pd, unsigned tam);
int  sacar_de_cola(tCola *pc, void *pd, unsigned tam);
int  frente_de_cola(const tCola *pc, void *dato, unsigned tam);
int  cola_llena(const tCola *pc, unsigned tam);
int  cola_vacia(const tCola *pc);
void vaciar_cola(tCola *pc);

#endif // COLA_H_INCLUDED
