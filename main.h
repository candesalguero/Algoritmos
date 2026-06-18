#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "Juego/config.h"
#include "Juego/funciones.h"
#include "Juego/juego.h"
#include "Lotes_Prueba/lotes_de_prueba.h"
/*/ ARCHIVOS ///////////////////////////////*/
#define ARCHIVO_CONFIG "Archivos/config.txt"
#define ARCHIVO_TABLERO "Archivos/caravana.txt"
#define ARCHIVO_JUGADORES "Archivos/jugadores.dat"
#define ARCHIVO_PARTIDAS "Archivos/partidas.dat"
#define ARCHIVO_INDICE "Archivos/indiceJugadores.idx"


int GuardarArchivo(const char *arch, const void *vec, size_t tamElem,size_t cant );
int MostrarArchivo(const char *arch, size_t tamElem, tMostrar mostrar);
void mostrarJugador(const void *jugadores);
void mostrarPartida(const void *partida);
void mostrarIdx(const void *jug);
int cmpJugadoresIdx(const void *jugador1, const void *jugador2);
void CopiarIndice(void *dest, const void *orig, const void *reg);
int ActualizarJugadorGuardado(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores);
int GuardarNuevaPartida(tPartida *nuevaPartida, const char *archPartidas);

#endif /*/ MAIN_H_INCLUDED*/
