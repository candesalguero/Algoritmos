#include "cola_dinamica.h"

void crearCola(tCola *p)
{
    p->pri = NULL;
    p->ult = NULL;
}

int colaLlena(const tCola *p, unsigned cantBytes)
{
    return DISPONIBLE;
}

int ponerEnCola(tCola *p, const void *elem, unsigned cantBytes)
{
    tNodo *nue = (tNodo *) malloc(sizeof(tNodo));

    if(nue == NULL || (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, elem, cantBytes);
    nue->tamElem = cantBytes;
    nue->sig = NULL;

    if(p->ult)
        p->ult->sig = nue;
    else
        p->pri = nue;
    p->ult = nue;

    return TODO_OK;
}

int verPrimeroCola(const tCola *p, void *elem, unsigned cantBytes)
{
    if(p->pri == NULL)
        return COLA_VACIA;

    memcpy(elem, p->pri->info, MIN(cantBytes, p->pri->tamElem));
    return TODO_OK;
}

int colaVacia(const tCola *p)
{
    return p->pri == NULL;
}

int sacarDeCola(tCola *p, void *elem, unsigned cantBytes)
{
    tNodo *aux = p->pri;
    if(aux == NULL)
        return COLA_VACIA;

    p->pri = aux->sig;
    memcpy(elem, aux->info, MIN(aux->tamElem, cantBytes));
    free(aux->info);
    free(aux);

    if(p->pri == NULL)
        p->ult = NULL;

    return TODO_OK;
}

void vaciarCola(tCola *p)
{
    while(p->pri)
    {
        tNodo *aux = p->pri;
        p->pri = aux->sig;
        free(aux->info);
        free(aux);
    }

    p->ult = NULL;
}
