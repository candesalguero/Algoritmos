#include "../Headers/ListaDoble.h"
#include <stdlib.h>
#include <string.h>

// 1. Crear Lista
void crearLista(tListaDoble *pl) {
    *pl = NULL;
}

// 2. Vaciar Lista
int vaciarLista(tListaDoble *pl) {
    int cant = 0;
    tNodo *actual = *pl;
    if (actual) {
        tNodo *ultimo = actual->anterior;
        ultimo->siguiente = NULL; 
        while (actual) {
            tNodo *aux = actual->siguiente;
            free(actual->info);
            free(actual);
            actual = aux;
            cant++;
        }
    }
    *pl = NULL;
    return cant;
}

// 3. Lista Vacía
int listaVacia(const tListaDoble *pl) {
    return *pl == NULL;
}

// 4. Lista Llena
int listallena(const tListaDoble pl, unsigned cantBytes) {
    tNodo *nuevo = (tNodo *)malloc(sizeof(tNodo));
    void *aux = malloc(cantBytes);
    free(nuevo);
    free(aux);
    return aux == NULL || nuevo == NULL;
}

// 5. Insertar al Final
int insertarAlFinal(tListaDoble *pl, const void *d, unsigned cantBytes) {
    tNodo *nuevo;
    if ((nuevo = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
        (nuevo->info = malloc(cantBytes)) == NULL) {
        free(nuevo);
        return 0;
    }
    memcpy(nuevo->info, d, cantBytes);
    nuevo->tamInfo = cantBytes;
    if (*pl == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        *pl = nuevo;
    } else {
        tNodo *primero = *pl;
        tNodo *ultimo = primero->anterior;
        nuevo->siguiente = primero;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        primero->anterior = nuevo;
    }
    return 1;
}

// 6. Insertar al Comienzo
int insertarAlComienzo(tListaDoble *pl, const void *d, unsigned cantBytes) {
    if (!insertarAlFinal(pl, d, cantBytes)) return 0;
    *pl = (*pl)->anterior; 
    return 1;
}

// Nota: Las demas primitivas se agregan en el codigo real siguiendo el archivo entregado.
