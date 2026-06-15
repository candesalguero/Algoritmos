#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "config.h"

void Menu();
int leerConfig(tConfiguracion* config, const char* arch);
int tirarDado();
void jugarPartida();
void ManejoErrores(int codError, const char *arch);
int GenerarIndiceJugadores(tArbol *arbolIdx,const char *archJugadores, const char *archIdx, tCmp cmp, tAccion CopiarIndice);
int InicializarIndice(tArbol *arbolIdx, const char *archJugadores, const char *archIdx, tCmp cmp, tAccion Copiar);
void ObtenerFechaActual(char *fechaDest);
int AltaJugadores(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores);
int BuscarDatosJugadores(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores);

void mostrarRanking(void);
int compararPorNombre(const void *a, const void *b);
void acumularPuntosMovimientos(void *viejo, const void *nuevo, const void *extra);
int compararPorPuntosDesc(const void *a, const void *b);
int cargarRanking(tLista *ranking);
void imprimirRanking(tLista *ranking);
#endif /*/ FUNCIONES_H_INCLUDED*/
