#include "../Headers/funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- PRIMITIVAS DE LA COLA ---
void crearCola(tCola *pc) {
    pc->frente = NULL;
    pc->fondo = NULL;
}

int acolar(tCola *pc, const tMovimiento *d) {
    tNodoCola *nuevo = (tNodoCola*)malloc(sizeof(tNodoCola));
    if(!nuevo) return 0; // Sin memoria

    nuevo->info = *d;
    nuevo->sig = NULL;

    if(pc->fondo == NULL) {
        pc->frente = nuevo;
    } else {
        pc->fondo->sig = nuevo;
    }
    pc->fondo = nuevo;
    return 1;
}

int desencolar(tCola *pc, tMovimiento *d) {
    if(pc->frente == NULL) return 0; // Cola vacía

    tNodoCola *aux = pc->frente;
    *d = aux->info;

    pc->frente = aux->sig;
    if(pc->frente == NULL) {
        pc->fondo = NULL;
    }
    free(aux);
    return 1;
}

int colaVacia(const tCola *pc) {
    return pc->frente == NULL;
}
