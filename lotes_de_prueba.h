#ifndef LOTES_DE_PRUEBA_H_INCLUDED
#define LOTES_DE_PRUEBA_H_INCLUDED

#include "config.h"
#include "main.h"

int guardarJugadoresArchivo(const char *nombreArchivo, const tJugador jugadores[], int cant);
int guardarPartidasArchivo(const char *nombreArchivo, const tPartida partidas[], int cant);

typedef void (*tMostrarJugador)(const tJugador *);
typedef void (*tMostrarPartida)(const tPartida *);

int recorrerArchivoJugadores(const char *nombreArchivo, tMostrarJugador mostrar);
int recorrerArchivoPartidas(const char *nombreArchivo, tMostrarPartida mostrar);

#endif // LOTES_DE_PRUEBA_H_INCLUDED
