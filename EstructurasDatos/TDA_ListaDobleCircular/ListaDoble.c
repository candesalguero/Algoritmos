#include "ListaDoble.h"


void crearLista(tListaDoble *pl) {
    *pl = NULL;
}


int vaciarLista(tListaDoble *pl) {
    int cant = 0;
    tNodoDoble *actual = *pl;
    tNodoDoble *ultimo;
    tNodoDoble *aux;
    if (actual) {
        ultimo = actual->anterior;
        ultimo->siguiente = NULL;
        while (actual) {
            aux = actual->siguiente;
            free(actual->info);
            free(actual);
            actual = aux;
            cant++;
        }
    }
    *pl = NULL;
    return cant;
}


int listaVacia(const tListaDoble *pl) {
    return *pl == NULL;
}


int listallena(const tListaDoble pl, unsigned cantBytes) {
    tNodo *nuevo = (tNodo *)malloc(sizeof(tNodo));
    void *aux = malloc(cantBytes);
    free(nuevo);
    free(aux);
    return aux == NULL || nuevo == NULL;
}


int insertarAlFinal(tListaDoble *pl, const void *d, unsigned cantBytes) {
    tNodoDoble *nuevo;
    tNodoDoble *primero;
    tNodoDoble *ultimo;

    if ((nuevo = (tNodo *)malloc(sizeof(tNodo))) == NULL ||
        (nuevo->info = malloc(cantBytes)) == NULL) {
        free(nuevo);
        return SIN_MEM;
    }
    memcpy(nuevo->info, d, cantBytes);
    nuevo->tamInfo = cantBytes;
    if (*pl == NULL) {
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        *pl = nuevo;
    } else {
        primero = *pl;
        ultimo = primero->anterior;
        nuevo->siguiente = primero;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        primero->anterior = nuevo;
    }
    return TODO_OK;
}


int insertarAlComienzo(tListaDoble *pl, const void *d, unsigned cantBytes) {
    if (!insertarAlFinal(pl, d, cantBytes))
        return SIN_MEM;
    *pl = (*pl)->anterior;
    return TODO_OK;
}

int BuscarElementoListaDC(const tListaDoble *lista, const void *aBuscar, void *elem, size_t tamElem, tCmp cmp)
{
    tNodo *act;
    act = *lista;

    if(!act)
        return LISTA_VACIA;
    do
    {
        if(cmp(act->info, aBuscar)==0)
        {
            memcpy(elem,act->info,MIN(tamElem, act->tamElem));
            return TODO_OK;
        }
        act = act->sig;
    }while(act != *lista);

    return NOT_FOUND_ELEM;
}



