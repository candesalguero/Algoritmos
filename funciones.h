#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "config.h"

void Menu();
int leerConfig(tConfiguracion* config, const char* arch);
int tirarDado();
void jugarPartida();
void mostrarRanking(void);
void ManejoErrores(int codError, const char *arch);
int GenerarIndiceJugadores(tArbol *arbolIdx,const char *archJugadores, const char *archIdx, tCmp cmp, tAccion CopiarIndice);
void ObtenerFechaActual(char *fechaDest);
int AltaJugadores(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores);
int BuscarDatosJugadores(tJugador *jugador, tJugadorIdx *nuevoJugador, const char *archJugadores);

#endif /*/ FUNCIONES_H_INCLUDED*/
