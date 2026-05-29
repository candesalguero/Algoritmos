#ifndef CONFIGESTRUCTURAS_H_INCLUDED
#define CONFIGESTRUCTURAS_H_INCLUDED

#include "config.h"

//Etiquetas de Return
#define SIN_MEM -2
#define LISTA_VACIA -3
#define NOT_FOUND_ELEM -4
#define COLA_LLENA -5
#define COLA_VACIA -6


typedef struct sNodo
{
    void *info;
    size_t tamElem;
    struct sNodo *sig;
}tNodo;

typedef struct sNodoDoble
{
    void *info;
    size_t tamElem;
    struct sNodoDoble *sig;
    struct sNodoDoble *ant;
}tNodoDoble;

typedef struct sNodoArbol
{
    void *info;
    size_t tamElem;
    struct sNodoArbol *sig;
    struct sNodoArbol *ant;
}tNodoArbol;


typedef struct
{
    tNodo *pri;
    tNodo *ult;
}tCola;

typedef tNodo *tLista;
typedef tNodoDoble *tListaDoble;
typedef tNodoArbol *arbol;

#endif // CONFIGESTRUCTURAS_H_INCLUDED
