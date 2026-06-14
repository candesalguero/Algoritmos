#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

#include "config.h"
#include "funciones.h"
#include "lotes_de_prueba.h"

int GuardarArchivo(const char *arch, const void *vec, size_t tamElem,size_t cant );
int MostrarArchivo(const char *arch, size_t tamElem, tMostrar mostrar);

#endif /*/ MAIN_H_INCLUDED*/
