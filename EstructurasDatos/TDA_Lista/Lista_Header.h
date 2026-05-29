#ifndef LISTA_HEADER_H_INCLUDED
#define LISTA_HEADER_H_INCLUDED

#include "../../configEstructuras.h"

void CrearLista(tLista *lista);
int InsertarAlPrincipio(tLista *lista, const void*elem, size_t tamElem);
int InsertarAlFinal(tLista *lista, const void *elem, size_t tamElem);
int InsertarOrdenado(tLista *lista, const void *elem, size_t tamElem, tCmp cmp, int conDup, tAccion accion);
int MostrarLista(tLista *lista, tMostrar mostrar);
int EliminarPorClaveOrdenada(tLista *lista, const void* clave, void* elem, size_t tamElem ,tCmp cmp);
int VaciarLista(tLista *lista);
void OrdenarLista(tLista *lista, tCmp cmp);
#endif // LISTA_HEADER_H_INCLUDED
