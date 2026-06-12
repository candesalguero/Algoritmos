#include "lotes_de_prueba.h"

int guardarJugadoresArchivo(const char *nombreArchivo, const tJugador jugadores[], int cant)
{
    FILE *pf = fopen(nombreArchivo, "wb");

    if(pf == NULL)
    {
        return ERR_ARCH;
    }

    fwrite(jugadores, sizeof(tJugador), cant, pf);

    fclose(pf);

    return TODO_OK;
}

int guardarPartidasArchivo(const char *nombreArchivo, const tPartida partidas[], int cant)
{
    FILE *pf = fopen(nombreArchivo, "wb");

    if(pf == NULL)
    {
        return ERR_ARCH;
    }

    fwrite(partidas, sizeof(tPartida), cant, pf);

    fclose(pf);

    return TODO_OK;
}

int recorrerArchivoJugadores(const char *nombreArchivo, tMostrarJugador mostrar)
{
    FILE *pf = fopen(nombreArchivo, "rb");
    tJugador jugador;

    if(pf == NULL)
        return ERR_ARCH;

    while(fread(&jugador, sizeof(tJugador), 1, pf) == 1)
    {
        mostrar(&jugador);
    }

    fclose(pf);

    return TODO_OK;
}

int recorrerArchivoPartidas(const char *nombreArchivo, tMostrarPartida mostrar)
{
    FILE *pf = fopen(nombreArchivo, "rb");
    tPartida partida;

    if(pf == NULL)
        return ERR_ARCH;

    while(fread(&partida, sizeof(tPartida), 1, pf) == 1)
    {
        mostrar(&partida);
    }

    fclose(pf);

    return TODO_OK;
}

