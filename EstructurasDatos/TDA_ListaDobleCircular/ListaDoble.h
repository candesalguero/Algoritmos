#ifndef LISTADOBLE_H
#define LISTADOBLE_H

#include "../../config.h"
#define CLA_DUP -1

typedef struct sNodoDoble {
    void *info;
    unsigned tamInfo;
    struct sNodoDoble *siguiente, *anterior;
} tNodoDoble;

typedef tNodoDoble *tListaDoble;

void crearLista(tListaDoble *pl);
int vaciarLista(tListaDoble *pl);
int listaVacia(const tListaDoble *pl);
int listallena(const tListaDoble pl, unsigned cantBytes);
int insertarAlFinal(tListaDoble *pl, const void *d, unsigned cantBytes);
int insertarAlComienzo(tListaDoble *pl, const void *d, unsigned cantBytes);
int mostrarDeIzqADer(const tListaDoble *pl, void (*mostrar)(const void *));
int mostrarDeDerAIzq(const tListaDoble *pl, void (*mostrar)(const void *));
int insertarEnOrden(tListaDoble *pl, const void *d, unsigned cantBytes, int (*comparar)(const void *, const void *), int (*acumular)(void **, unsigned *, const void *, unsigned));
void ordenarLista(tListaDoble *pl, int (*comparar)(const void *, const void *));
int eliminarPorClave(tListaDoble *pl, void *d, unsigned cantBytes, int (*comparar)(const void *, const void *));
int sacarDelInicio(tListaDoble *p, void *d, unsigned cantBytes);
int sacarDelFinal(tListaDoble *p, void *d, unsigned cantBytes);
int buscarYVer(const tListaDoble *p, void *d, unsigned cantBytes, int (*comparar)(const void *, const void *));
int buscarYSacar(tListaDoble *p, void *d, unsigned cantBytes, int (*comparar)(const void *, const void *));

#endif
