#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "config.h"

typedef struct
{
    char nickname[30];
    int  puntosTotal;
    int  movimientosTotal;
}tRankingItem;

int compararPorNombre(const void *a, const void *b);
void acumularPuntosMovimientos(void *viejo, const void *nuevo);
int compararPorPuntosDesc(const void *a, const void *b);
void mostrarRanking(void);
void imprimirRanking(tLista *ranking);
int cargarRanking(tLista *ranking);

#endif // RANKING_H_INCLUDED
