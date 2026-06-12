#include "ranking.h"
#include "Lista_Header.h"
#include "lotes_de_prueba.h"

int compararPorNombre(const void *a, const void *b)
{
    const tRankingItem *ja = (const tRankingItem *)a;
    const tRankingItem *jb = (const tRankingItem *)b;

    return strcmp(ja->nickname, jb->nickname);
}

void acumularPuntosMovimientos(void *viejo, const void *nuevo)
{
    tRankingItem *jViejo = (tRankingItem *)viejo;
    const tRankingItem *jNuevo = (const tRankingItem *)nuevo;

    jViejo->puntosTotal += jNuevo->puntosTotal;
    jViejo->movimientosTotal += jNuevo->movimientosTotal;
}

int compararPorPuntosDesc(const void *a, const void *b)
{
    const tRankingItem *ja = (const tRankingItem *)a;
    const tRankingItem *jb = (const tRankingItem *)b;

    // Caso normal, el que tiene más puntos va primero (descendente)
    if(jb->puntosTotal != ja->puntosTotal)
        return jb->puntosTotal - ja->puntosTotal;

    // Caso empate, gana el que hizo MENOS movimientos (ascendente)
    return ja->movimientosTotal - jb->movimientosTotal;
}

void mostrarRanking(void)
{
    tLista ranking;

    CrearLista(&ranking);

    if(cargarRanking(&ranking) == ERR_ARCH)
    {
        printf("\nNo hay partidas registradas todavia.\n");
        return;
    }

    OrdenarLista(&ranking, compararPorPuntosDesc);
    imprimirRanking(&ranking);
    VaciarLista(&ranking);

}

int cargarRanking(tLista *ranking)
{
    FILE *pf;
    tPartida partida;
    tRankingItem item;

    pf = fopen(ARCHIVO_PARTIDAS, "rb");
    if(pf == NULL)
        return ERR_ARCH;

    while(fread(&partida, sizeof(tPartida), 1, pf) == 1)
    {
        strcpy(item.nickname, partida.nickname);
        item.puntosTotal = partida.puntos;
        item.movimientosTotal = partida.cantMovimientos;

        InsertarOrdenado(ranking, &item, sizeof(tRankingItem),compararPorNombre, 0, acumularPuntosMovimientos);
    }

    fclose(pf);
    return TODO_OK;
}

void imprimirRanking(tLista *ranking)
{
    tNodo *actual;
    tRankingItem *item;
    int puesto;

    printf("\n============= RANKING =============\n");
    printf("%-8s %-20s %-8s\n", "Puesto", "Jugador", "Puntos");

    actual = *ranking;
    puesto = 1;

    while(actual != NULL)
    {
        item = (tRankingItem *)actual->info;
        printf("%-8d %-20s %-8d\n", puesto, item->nickname, item->puntosTotal);

        actual = actual->sig;
        puesto++;
    }
    printf("===================================\n");
}
