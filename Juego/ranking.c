#include "config.h"
#include "funciones.h"
#include "../main.h"


int compararPorNombre(const void *a, const void *b)
{
    const tRankig *ja = (const tRankig *)a;
    const tRankig *jb = (const tRankig *)b;

    return strcmp(ja->nickname, jb->nickname);
}


void acumularPuntosMovimientos(void *viejo, const void *nuevo, const void *extra)
{
    tRankig *jViejo = (tRankig *)viejo;
    const tRankig *jNuevo = (const tRankig *)nuevo;
    (void)extra; /* para solucionar el warning de la firma */
    jViejo->puntos_totales += jNuevo->puntos_totales;
    jViejo->mov_totales += jNuevo->mov_totales;
}

int compararPorPuntosDesc(const void *a, const void *b)
{
    const tRankig *ja = (const tRankig *)a;
    const tRankig *jb = (const tRankig *)b;

    /* Mas puntos primero (descendente) */
    if(ja->puntos_totales != jb->puntos_totales)
        return (jb->puntos_totales > ja->puntos_totales) ? 1 : -1;

    /* Empate: menos movimientos primero (ascendente) */
    if(ja->mov_totales != jb->mov_totales)
        return (ja->mov_totales > jb->mov_totales) ? 1 : -1;

    return 0;
}

/* Lee partidas.dat y arma la lista acumulando puntos/movimientos por jugador */
int cargarRanking(tLista *ranking)
{
    FILE *pf;
    tPartida partida;
    tRankig item;

    pf = fopen(ARCHIVO_PARTIDAS, "rb");
    if(pf == NULL)
        return ERR_ARCH;

    while(fread(&partida, sizeof(tPartida), 1, pf) == 1)
    {
        strcpy(item.nickname, partida.nickname);
        item.puntos_totales = partida.puntos;
        item.mov_totales = partida.cant_movimientos;

        InsertarOrdenado(ranking, &item, sizeof(tRankig), compararPorNombre, 0, acumularPuntosMovimientos);
    }

    fclose(pf);
    return TODO_OK;
}

/* Recorre la lista (ya ordenada) y muestra la tabla con los puestos */
void imprimirRanking(tLista *ranking)
{
    tNodo *actual;
    tRankig *item;
    int puesto;

    printf("\n================= RANKING =================\n");
    printf("%-8s %-20s %-8s %-8s\n", "Puesto", "Jugador", "Puntos", "Movim.");

    actual = *ranking;
    puesto = 1;
    while(actual != NULL)
    {
        item = (tRankig *)actual->info;
        printf("%-8d %-20s %-8u %-8u\n", puesto, item->nickname,item->puntos_totales, item->mov_totales);

        actual = actual->sig;
        puesto++;
    }
    printf("==========================================\n");
}

/* Funcion principal: crea, carga, ordena, muestra y libera */
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
