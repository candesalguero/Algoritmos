#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "config.h"
#include "funciones.h"
#include "EstructurasDatos/TDA_Cola/cola_dinamica.h"
#include "EstructurasDatos/TDA_Lista/Lista_Header.h"

int GuardarArchivo(const char *arch, const void *vec, size_t tamElem,size_t cant );
int MostrarArchivo(const char *arch, size_t tamElem, tMostrar mostrar);
int guardarJugadoresArchivo(const char *nombreArchivo, const tJugador jugadores[], int cant);
int guardarPartidasArchivo(const char *nombreArchivo, const tPartida partidas[], int cant);
void mostrarPartida(const void *part);
#endif /*/ MAIN_H_INCLUDED*/
