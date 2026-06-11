#ifndef ARBOL_HEADER_H_INCLUDED
#define ARBOL_HEADER_H_INCLUDED


#include "../../config.h"


#define ARBOL_VACIO -2
#define PRE_ORDEN 0
#define IN_ORDEN 1

typedef struct sNodoArbol
{
    void *info;
    size_t tamElem;
    struct sNodoArbol *izq;
    struct sNodoArbol *der;
}tNodoArbol;

typedef tNodoArbol *tArbol;


typedef int(*tCmp)(const void*, const void*);//funcion de comparacion
typedef void(*tAccion)(void*, const void*, const void*);
typedef void(*tMostrar)(const void*);

void CrearArbol(tArbol *);
void TalarArbol(tArbol *arbol);
int AlturaArbol(tArbol *arbol);
int InsertarOrdenadoEnArbol(tArbol *arbol, const void* elem, size_t tamElem, tCmp cmp);
void RecorrerInOrden(const tArbol *arbol, tMostrar mostrar);
void RecorrerPreOrden(const tArbol *arbol, tMostrar mostrar);

int EliminarNodo(tArbol *arbol, const void *aElim, void *elem, size_t tamElem, tCmp cmp);
void CortarHoja(tArbol *arbol);
void EliminarRaiz(tArbol *arbol);
tNodoArbol **_BuscarMayorClave(tArbol *arbol);
tNodoArbol **_BuscarMenorClave(tArbol *arbol);

///FUNCIONES ARBOL CON ARCHIVOS
int CargarArchivo_fromArbol(const tArbol *arbol, const char *arch, int metodo);
void _CargarArchInOrden(const tArbol *arbol, FILE *pf);
void _CargarArchPreOrden(const tArbol *arbol, FILE *pf);

int ArbolBalanceado_fromArchOrdenado(tArbol *arbol,const char *arch, size_t tamElemOr, size_t tamElemIdx ,tAccion accion );
void _InsersionBalanceada(tArbol *arbol, void *elemArch ,size_t tamElemOr, size_t tamElemIdx ,int base, int tope, FILE *pf, tAccion accion );
int BajarArchivoArbol(tArbol *arbol, const char *arch,size_t tamElem ,tCmp cmp);





#endif // ARBOL_HEADER_H_INCLUDED
