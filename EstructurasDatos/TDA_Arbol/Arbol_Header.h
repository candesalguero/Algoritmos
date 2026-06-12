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

/*
typedef int(*tCmp)(const void*, const void*);
typedef void(*tAccion)(void*, const void*, const void*);
typedef void(*tMostrar)(const void*);
*/
void Arbol_Crear(tArbol *);
int Arbol_Insertar(tArbol *arbol, const void* elem, size_t tamElem, tCmp cmp);
int Arbol_ObtenerAltura(tArbol *arbol);
void Arbol_RecorrerInOrden(const tArbol *arbol, tMostrar mostrar);
void Arbol_RecorrerPreOrden(const tArbol *arbol, tMostrar mostrar);
void Arbol_Destruir(tArbol *arbol);
int Arbol_BusquedaBinaria(const tArbol *arbol, const void *aBuscar, void *encontrado, size_t tamEncontrado, tCmp cmp);


int Arbol_EliminarNodo(tArbol *arbol, const void *aElim, void *elem, size_t tamElem, tCmp cmp);
void _Arbol_EliminarHoja(tArbol *arbol);
void _Arbol_EliminarRaiz(tArbol *arbol);
tNodoArbol **_Arbol_BuscarMayorClave(tArbol *arbol);
tNodoArbol **_Arbol_BuscarMenorClave(tArbol *arbol);

/**FUNCIONES ARBOL CON ARCHIVOS*/
int Arbol_GuardarEnArchivo(const tArbol *arbol, const char *arch, int metodo);
void _Arbol_GuardarArchInOrden(const tArbol *arbol, FILE *pf);
void _Arbol_GuardarArchPreOrden(const tArbol *arbol, FILE *pf);

int Arbol_GenerarIndiceBalanceado(tArbol *arbol,const char *arch, size_t tamElemOr, size_t tamElemIdx ,tAccion accion );
int _Arbol_InsertarBalanceado(tArbol *arbol, void *elemArch ,size_t tamElemOr, size_t tamElemIdx ,int base, int tope, FILE *pf, tAccion accion );
int Arbol_CargarDesdeArchivo(tArbol *arbol, const char *arch,size_t tamElem ,tCmp cmp);





#endif /* ARBOL_HEADER_H_INCLUDED*/
