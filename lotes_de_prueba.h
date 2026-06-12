#ifndef LOTES_DE_PRUEBA_H_INCLUDED
#define LOTES_DE_PRUEBA_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include "config.h"

typedef struct
{
    char nickname[30];
    char fecha[11]; // formato: dd/mm/aaaa
    int cantPartidasJugadas;
} tJugador;

typedef struct
{
    int nroPartida;
    char nickname[30]; // debe coincidir con nickname de tJugador
    int puntos;
    int cantMovimientos;
} tPartida;

int guardarJugadoresArchivo(const char *nombreArchivo, const tJugador jugadores[], int cant);
int guardarPartidasArchivo(const char *nombreArchivo, const tPartida partidas[], int cant);

typedef void (*tMostrarJugador)(const tJugador *);
typedef void (*tMostrarPartida)(const tPartida *);

int recorrerArchivoJugadores(const char *nombreArchivo, tMostrarJugador mostrar);
int recorrerArchivoPartidas(const char *nombreArchivo, tMostrarPartida mostrar);

#endif // LOTES_DE_PRUEBA_H_INCLUDED
